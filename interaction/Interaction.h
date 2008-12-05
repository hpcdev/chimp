
#ifndef PARTICLEDB_INTERACTION_INTERACTION_H
#define PARTICLEDB_INTERACTION_INTERACTION_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <ostream>
#include <set>
#include <string>

#include "CrossSection.h"
#include "VHSCrossSection.h"
#include "DATACrossSection.h"
#include "../Particle.h"


namespace particledb { namespace interaction {

    /** Input data type indices. */
    struct Input {
        int A;
        int B;

        template <class RnDB>
        std::ostream & print(std::ostream & out, const RnDB & db) const {
            using std::string;
            const Particle::property::name & n_A = db[A];
            const Particle::property::name & n_B = db[B];
            const Particle::property::mass & m_A = db[A];
            const Particle::property::mass & m_B = db[B];

            const string * n0 = &n_A.value;
            const string * n1 = &n_B.value;
            if (m_A.value > m_B.value) {
                n0 = &n_B.value;
                n1 = &n_A.value;
            }

            out << '(' << (*n0) << ")"
                  "+(" << (*n1) << ')';

            return out;
        }
    };

    /** Output information of interactions. */
    struct Output {
        struct item {
            int n;
            int type;
        };
        typedef std::vector<item> item_list;

        item_list items;

        boost::shared_ptr<CrossSection> cs;

        struct name_mass {
            const Particle::property::mass & mass;
            const Particle::property::name & name;

            name_mass(const Particle::property::mass & m,
                      const Particle::property::name & n)
                : mass(m), name(n) {}

            bool operator<(const name_mass & rhs) const {
                return this->mass.value < rhs.mass.value;
            }
        };
        typedef std::multiset<name_mass> printset;

        template <class RnDB>
        std::ostream & print(std::ostream & out, const RnDB & db) const {
            printset ps;
            for (item_list::const_iterator i = items.begin(); i!=items.end(); i++) {
                const Particle::property::mass & m = db[i->type];
                const Particle::property::name & n = db[i->type];
                ps.insert(name_mass(m,n));
            }

            const char * plus = "+";
            const char * sep = "";
            for (printset::iterator i = ps.begin(); i!=ps.end(); i++) {
                out << sep << '(' << i->name.value << ")";
                sep = plus;
            }
            return out;
        }

        /** Compute the cross section.
         * @param v_relative
         *     The relative velocity of the two particles in question.
         * */
        double cross_section(const double & v_relative) const {
            return cs->cross_section(v_relative);
        }

        /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
        double effective_radius(const double & v_relative) const {
            return cs->effective_radius(v_relative);
        }
    };

    struct Equation : Input, Output {
        typedef std::vector<Equation> list;

        template <class RnDB>
        std::ostream & print(std::ostream & out, const RnDB & db) const {
            Input::print(out,db) << "->";
            Output::print(out,db);
            return out;
        }

        template <class RnDB>
        static Equation load(const xml::XMLContext & x,
                         const std::string & Eq,
                         const RnDB & db) {
            return Equation::load(
                x.find("//Interaction[string(Eq)='" + Eq + "']"),
                db
            );
        }

        template <class RnDB>
        static Equation load(const xml::XMLContext & x, const RnDB & db) {
            using xml::XMLContext;
            using xml::xml_error;
            using std::string;
            using boost::shared_ptr;

            Equation retval;
            XMLContext::list xl = x.eval("Eq/In/P");

            if (xl.size() == 2) {
                XMLContext::list::iterator i = xl.begin();
                retval.A = db.findParticle(i->parse<string>());
                retval.B = db.findParticle((++i)->parse<string>());
                if (retval.A == -1 || retval.B == -1)
                    throw xml_error(
                        "Cannot load equation with unknown inputs. "
                        "please add inputs to RuntimeDB instance first."
                    );
            } else if (xl.size() == 1) {
                /* single species elastic collision */
                if (xl.begin()->query<int>("@M") != 2)
                    throw xml_error("(too few inputs) Binary interaction must have two inputs");
                retval.A = retval.B = db.findParticle(xl.begin()->parse<string>());
            } else
                throw xml_error("(too many inputs) Binary interaction must have two inputs");

            /* now determine the output particles and their respective
             * multipliers. */
            xl = x.eval("Eq/Out/P");
            for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
                Output::item it = {0, db.findParticle(i->parse<string>())};

                if (it.type == -1)
                    throw xml_error(
                        "Cannot load equation with unknown outputs. "
                        "please add outputs to RuntimeDB instance first."
                    );

                try {
                    it.n = i->query<int>("@M");
                } catch (xml::no_results) {/* ignored */}

                retval.items.push_back(it);
            }

            /* now, instantiate the child CrossSection object with the correct
             * type. */
            XMLContext cs_x = x.find("cross_section");
            string cs_type = cs_x.query<string>("@type");

            /* masses of input items: m_A, m_B */
            const Particle::property::mass & m_A = db[retval.A];
            const Particle::property::mass & m_B = db[retval.B];
            double mu_AB = m_A.value * m_B.value / (m_A.value + m_B.value);

            if        (cs_type == "vhs/vss") {
                shared_ptr<CrossSection> a(new VHSCrossSection(VHSCrossSection::load(cs_x, mu_AB)));
                retval.cs = a;
            } else if (cs_type == "data") {
                shared_ptr<CrossSection> a(new DATACrossSection(DATACrossSection::load(cs_x, mu_AB)));
                retval.cs = a;
            } else {
                string Eq = x.query<string>("Eq");
                throw xml_error("no cross section found for interaction '" + Eq + '\'');
            }

            return retval;
        }
    };

    static inline xml::XMLContext::list filter_interactions(
            const xml::XMLContext::list & xl_in,
            const std::set<std::string> & allowed_equations) {
        using xml::XMLContext;
        using std::string;

        XMLContext::list xl_out;
        for (XMLContext::list::const_iterator i = xl_in.begin(); i!= xl_in.end(); i++) {
            if (allowed_equations.find(i->query<string>("Eq"))
                != allowed_equations.end())
            xl_out.push_back(*i);
        }

        return xl_out;
    }
            

    /** Find all interactions where A and B are inputs.
     * */
    static inline xml::XMLContext::list find_all_interactions(
            const xml::XMLContext & x,
            const std::string & A,
            const std::string & B) {
        std::string Eq_in;
        if (A == B)
            Eq_in = '(' + A + ')';
        else
            Eq_in = '(' + A + ")+(" + B + ')';
        /* find all equations that match the given inputs. */
        return x.eval("//Interaction[string(Eq/In)='" + Eq_in + "']");
    }

    /** Find all elastic interactions where A and B are inputs and outputs.
     * */
    static inline xml::XMLContext::list find_elastic_interactions(
            const xml::XMLContext & x,
            const std::string & A,
            const std::string & B) {
        std::string Eq;
        if (A == B)
            Eq = '(' + A + ")->(" + A + ')';
        else
            Eq = '(' + A + ")+(" + B + ")->(" + A + ")+(" + B + ')';
        /* find all equations that match the given inputs. */
        return x.eval("//Interaction[string(Eq)='" + Eq + "']");
    }

}}/* namespace particldb::interaction */
#endif // PARTICLEDB_INTERACTION_INTERACTION_H
