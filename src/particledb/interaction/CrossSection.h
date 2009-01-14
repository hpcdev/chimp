
#ifndef PARTICLEDB_INTERACTION_CROSSSECTION_H
#define PARTICLEDB_INTERACTION_CROSSSECTION_H

#include <cmath>
#include <vector>

#include <physical/physical.h>

#include "v_rel_fnc.h"

namespace particledb { namespace interaction {

    struct CrossSection {

        /** The factor to determine the assumed maximum relative speed of an
         * ensemble of particles based upon the standard deviation of the
         * velocity distribution.  For example, if
         * <code>MAX_SPEED_FACTOR = 3</code>, more than 9?% of all particles
         * will have a velocity lower than MAX_SPEED_FACTOR*stddev_v_rel.
         * FIXME:  calculate this number again and put it here.
         * */
        static const double MAX_SPEED_FACTOR = 3.0;

        /** Reduced mass for the input particles. */
        double reduced_mass;

        CrossSection(const double & mu = 0.0) : reduced_mass(mu) {}
        virtual ~CrossSection() {}

        /** Compute the cross section.
         * @param v_relative
         *     The relative velocity of the two particles in question.
         * */
        virtual double cross_section(const double & v_relative) const  = 0;

        /** Find the local maximum of cross-section*velocity (within a given
         * range of velocity space.
         * */
        virtual double find_max_sigma_v_rel(const double & v_rel_max) const = 0;

        /** Find the local maximum of cross-section*velocity assuming an
         * ensemble of particles with a given temperature.
         * */
        inline double find_max_sigma_v_rel_from_stddev_v(const double & stddev_v) const {
            return find_max_sigma_v_rel(MAX_SPEED_FACTOR * stddev_v);
        }

        /** Find the local maximum of cross-section*velocity assuming an
         * ensemble of particles with a given temperature.
         * */
        inline double find_max_sigma_v_rel_from_T(const double & T) const {
            return find_max_sigma_v_rel_from_stddev_v(stddev_v_rel(T, reduced_mass));
        }

        /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
        inline double effective_radius(const double & v_relative) const {
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
