
#ifndef PARTICLEDB_INTERACTION_H
#define PARTICLEDB_INTERACTION_H


#include <olson-tools/physical/physical.h>

#include "VHSCrossSection.h"
#include "DATACrossSection.h"

namespace particledb { namespace Interaction {

    /** Input data type indices. */
    struct Input {
        int A;
        int B;
    };

    struct CrossSection {
        virtual ~CrossSection() {}

        /** Compute the cross section.
         * @param v_relative
         *     The relative velocity of the two particles in question.
         * */
        virtual double cross_section(const double & v_relative) const  = 0;

        /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
        double effective_radius(const double & v_relative) const {
            using physical::unit::pi;
            return size * 0.5 * sqrt(cross_section(v_relative)/ pi);
        }
    };

    /** Output information of interactions. */
    struct Output {
        struct item {
            int n;
            int type;
        };

        Output() : cs(NULL) {}
        ~Output() {}

        std::vector<item> items;

        CrossSection * cs;

        /** Compute the cross section.
         * @param v_relative
         *     The relative velocity of the two particles in question.
         * */
        double cross_section(const double & v_relative) {
            return cs->cross_section(v_relative);
        }

        /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
        double effective_radius(const double & v_relative) const {
            return cs->effective_radius(v_relative);
        }
    };

    static inline XMLContext::list find_all_interactions(const XMLContext & x, const std::string & A, const std::string & B) {
        std::string Eq_in;
        if (A == B)
            Eq_in = '(' + A + ')';
        else
            Eq_in = '(' + A + ")+(" + B + ')';
        /* find all equations that match the given inputs. */
        return x.eval("//Interaction[string(Eq/In)='" + Eq_in + "']");
    }

    static inline XMLContext::list find_elastic_interactions(const XMLContext & x, const std::string & A, const std::string & B) {
        std::string Eq;
        if (A == B)
            Eq = '(' + A + ")->(" + A + ')';
        else
            Eq = '(' + A + ")+(" + B + ")->(" + A + ")+(" + B + ')';
        /* find all equations that match the given inputs. */
        return x.eval("//Interaction[string(Eq)='" + Eq + "']");
    }

    struct Info : Input, Output {
        template <class RnDB>
        static Info load(const XMLContext & x, const std::string & Eq, const RnDB & db) {
            return Info::load(
                x.find("//Interaction[string(Eq)='" + Eq + "']"),
                db
            );
        }

        template <class RnDB>
        static Info load(const XMLContext & x, const RnDB & db) {
            Info retval;
            XMLContext::list xl = x.eval("Eq/In/P");

            /* FIXME:  take care of case when the A == B and thus only one
             * input will be listed, but with a multiplier attribute of "M=2"
             * */
            if (xl.size() != 2)
                throw xml_error("Need exactly two inputs for a binary interaction");

            retval.A = db.findParticle(xl[0].parse<std::string>());
            retval.B = db.findParticle(xl[1].parse<std::string>());


            /* now determine the output particles and their respective
             * multipliers. */
            xl = x.eval("Eq/Out/P");
            for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
                Output::item it = {0, db.findParticle(i->parse<std::string>())};
                try {
                    it.n = i->query<int>("@M");
                } catch (xml::no_results) {/* ignored */}

                retval.items.push_back(it);
            }

            /* now, instantiate the child CrossSection object with the correct
             * type. */
            XMLContext cs_x = x.find("cross_section");
            std::string cs_type = cs_x.query<std::string>("@type");

            /* masses of input items: m_A, m_B */
            double mu_AB = m_A * m_B / (m_A + m_B);
            if        (cs_type == "vhs/vss") {
                cs = VHSCrossSection::load(cx_x, mu_AB);
            } else if (cs_type == "data") {
                cs = DATACrossSection::load(cx_x, mu_AB);
            }
        }
    };

    struct Set {
        Input lhs;
        std::vector<Output*> rhs;

        /** Return type of calcualteOutPath.
         * OutPath.first:  the index of the output interaction.<br>
         * OutPath.second:  the cross-section value for this output path.
         * */
        typedef std::pair<int, double> OutPath;

        /** Chooses an interaction path to traverse dependent on the incident
         * relative speed and the current value of (sigma*relspeed)_max. 
         *
         * The implementation here actually calculates the probabilities
         * fully during runtime. 
         *
         * @return The index of the right-hand-side of the interaction
         * equation is returned, unless no interaction can be performed.  In
         * this latter case, a value of -1 will be returned.
         * */
        int calculateOutPath(double & max_sigma_relspeed, const double & v_relative) const {
            /* first find the normalization factor for the sum of
             * cross-section values at this velocity. */
            double cs_tot = 0;
            double cs_max = 0;
            std::vector<double> cs;
            cs.reserve(rhs.size());
            for (std::vector<Output*>::iterator i = rhs.begin(); i != rhs.end(); i++) {
                double csi = i->cross_section(v_relative);
                cs_tot += csi;
                cs.push_back(csi);

                if (csi > cs_max)
                    cs_max = csi;
            }

            /* now evaluate whether any of these interactions should even
             * happen. */
            if ( (MTRNGrand() * max_sigma_relspeed) > (cs_tot*v_relative) )
                return -1; /* no interaction!!! */

            /* upgrade max_sigma_relspeed? */
            {
                double cs_max_relspeed = cs_max * v_relative;
                if (cs_max_relspeed > max_sigma_relspeed)
                    max_sigma_relspeed = cs_max_relspeed;
            }

            /* now, we finally pick our output state.  The 0.999999999 factor
             * is to ensure that r < cs_tot. */
            double r = MTRNGrand() * cs_tot * 0.999999999;
            cs_tot = 0;
            int j = 0;
            for (std::vector<double>::iterator i = cs.begin(); i < cs.end(); i++, j++) {
                cs_tot += (*i);
                if (cs_tot > r)
                    return std::make_pair(j,(*i));
            }

            /* we actually better never get here. */
            logger::log_severe("Interaction::Set::calculateOutPath reached invalid return");
            return -1;
        }
    };

    /** This set implementation pre-calculates the cross-section and
     * probabilities at various pre-determined energies and stores the
     * associated lookup-table for later use. */
    struct PreComputedSet : Set {

        struct elt {
            double cross_section_total;
            std::vector<double> cross_sections;
            olson_tools::Distribution distrib;
        };


        int calculateOutPath(double & max_sigma_relspeed, const double & v_relative) const {
            if (rhs.empty())
                return -1;
            return 1;
        }
    };

}}/* namespace particldb::Interaction */
#endif // PARTICLEDB_INTERACTION_H
