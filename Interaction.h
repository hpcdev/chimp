
#ifndef PARTICLEDB_INTERACTION_H
#define PARTICLEDB_INTERACTION_H


#include <olson-tools/physical/physical.h>

namespace particledb {

namespace Interaction {

    /** Input data type indices. */
    struct Input {
        int A;
        int B;
    };

    /** Output information of interactions. */
    struct Output {
        struct item {
            int n;
            int type;
        };

        virtual ~Output() {}

        std::vector<item> items;

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

    struct Info : Input, Output {};

    struct Set {
        Input lhs;
        std::vector<Output*> rhs;

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
                double csi = rhs.cross_section(v_relative);
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
                cs_tot += rhs.cross_section(v_relative);
                if (cs_tot > r)
                    return j;
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

}/* namespace Interaction */





/** The base class of all binary interactions (binary inputs).
 * */
struct Interaction {
    std::vector<Interaction *> load(const XMLContext & x, const std::string & A, const std::string & B) {
        std::string Eq_in;
        if (A == B)
            Eq_in = '(' + A + ')';
        else
            Eq_in = '(' + A + ")+(" + B + ')';
        /* find all equations that match the given inputs. */
        XMLContext::list xl = x.eval("//Interaction[string(Eq/In)='" + Eq_in + "']");

        std::vector<Interaction *> ilist;

        for (XMLKContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
            /* first see if this interaction is allowed. */
            XMLContext & xi = (*i);
            std::string Eq = xi.query<std::sring>("Eq");

            if (!allowed)
                continue;

            XMLContext cs_x = xi.find("cross_section");
            std::string cs_type = cs_x.query<std::string>("@type");

            if        (cs_type == "vhs/vss") {
            } else if (cs_type == "data") {
            }
        }
    }
};

} /* particledb namespace .*/

#endif // PARTICLEDB_INTERACTION_H
