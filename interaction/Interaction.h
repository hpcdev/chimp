
#ifndef PARTICLEDB_INTERACTION_INTERACTION_H
#define PARTICLEDB_INTERACTION_INTERACTION_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <ostream>
#include <list>
#include <string>

#include "CrossSection.h"
#include "VHSCrossSection.h"
#include "DATACrossSection.h"
#include "../Particle.h"
#include "v_rel_fnc.h" /* include to make available by default. */


namespace particledb { namespace interaction {

    /** Input data type indices. */
    struct Input {
        int A;
        int B;
        double mu_AB;

        Input(const int & A = 0, const int & B = 0,
              const double & mu_AB = 0.0) : A(A), B(B), mu_AB(mu_AB) {}

        template <class RnDB>
        Input(const RnDB & db, const int & A = 0, const int & B = 0) :
            A(A), B(B) { set_mu_AB(db); }

        template <class RnDB>
        void set_mu_AB(const RnDB & db) {
            using Particle::property::mass;
            const double & m_A = db[A].mass::value;
            const double & m_B = db[B].mass::value;
            mu_AB = m_A * m_B / (m_A + m_B);
        }

        template <class RnDB>
        std::ostream & print(std::ostream & out, const RnDB & db) const {
            using std::string;
            using Particle::property::mass;
            using Particle::property::name;
            const string & n_A = db[A].name::value;
            const string & n_B = db[B].name::value;
            const double & m_A = db[A].mass::value;
            const double & m_B = db[B].mass::value;

            const string * n0 = &n_A;
            const string * n1 = &n_B;
            if (m_A > m_B) {
                n0 = &n_B;
                n1 = &n_A;
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
            using Particle::property::mass;

            Equation retval;
            XMLContext::set xl = x.eval("Eq/In/P");

            if (xl.size() == 2) {
                XMLContext::set::iterator i = xl.begin();
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

            retval.set_mu_AB(db);

            /* now determine the output particles and their respective
             * multipliers. */
            xl = x.eval("Eq/Out/P");
            for (XMLContext::set::iterator i = xl.begin(); i != xl.end(); i++) {
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

            if        (cs_type == "vhs/vss") {
                shared_ptr<CrossSection> a(
                    new VHSCrossSection(
                        VHSCrossSection::load(cs_x, retval.mu_AB)
                    )
                );
                retval.cs = a;
            } else if (cs_type == "data") {
                shared_ptr<CrossSection> a(
                    new DATACrossSection(
                        DATACrossSection::load(cs_x, retval.mu_AB)
                    )
                );
                retval.cs = a;
            } else {
                string Eq = x.query<string>("Eq");
                throw xml_error("no cross section found for interaction '" + Eq + '\'');
            }

            return retval;
        }
    };

    static inline xml::XMLContext::set filter_interactions(
            const xml::XMLContext::set & xl_in,
            const std::set<std::string> & allowed_equations) {
        using xml::XMLContext;
        using std::string;

        XMLContext::set xl_out;
        for (XMLContext::set::const_iterator i = xl_in.begin(); i!= xl_in.end(); i++) {
            if (allowed_equations.find(i->query<string>("Eq"))
                != allowed_equations.end())
            xl_out.insert(*i);
        }

        return xl_out;
    }
            

    /** Find all interactions where A and B are inputs.
     * NOTE:  According to the standard Equation format in the xml file, A and
     * B should be in order of mass, with mass(A) < mass(B).
     * B should be in order of mass, with mass(A) < mass(B).
     * This function does not reorder A and B.
     * */
    static inline xml::XMLContext::set find_all_interactions(
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
     * NOTE:  According to the standard Equation format in the xml file, A and
     * B should be in order of mass, with mass(A) < mass(B).
     * This function does not reorder A and B.
     * */
    static inline xml::XMLContext::set find_elastic_interactions(
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
