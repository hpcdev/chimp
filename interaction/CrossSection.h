
#ifndef PARTICLEDB_INTERACTION_CROSSSECTION_H
#define PARTICLEDB_INTERACTION_CROSSSECTION_H

#include <cmath>
#include <vector>

#include <olson-tools/physical/physical.h>

namespace particledb { namespace interaction {

    struct CrossSection {
        /** Reduced mass for the input particles. */
        double reduced_mass;

        CrossSection(const double & mu = 0.0) : reduced_mass(mu) {}
        virtual ~CrossSection() {}

        /** Compute the cross section.
         * @param v_relative
         *     The relative velocity of the two particles in question.
         * */
        virtual double cross_section(const double & v_relative) const  = 0;

        /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
        double effective_radius(const double & v_relative) const {
#ifndef PHYSICAL_QUANTITY_H
            using physical::unit::pi;
            return /*size * */ 0.5 * sqrt(cross_section(v_relative)/ pi);
#else
            throw std::logic_error("physical/quantity.h should NOT have been included at this point");
#endif
        }
    };

}}/* namespace particldb::interaction */
#endif // PARTICLEDB_INTERACTION_CROSSSECTION_H
