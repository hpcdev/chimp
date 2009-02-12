#ifndef PARTICLEDB_INTERACTION_V_REL_FNC_H
#define PARTICLEDB_INTERACTION_V_REL_FNC_H

#include <physical/physical.h>

namespace particledb { namespace interaction {

    /** Calculate the average relative velocity for a set of thermalized
     * particles at a given temperature and reduced mass.
     *
     * @param T
     *     Temperature of assumed ensemble.
     * @param reduced_mass
     *     Reduced mass of particles in assumed ensemble.
     * */
    inline double avg_v_rel(const double & T, const double & reduced_mass) {
        using physical::constant::K_B;
        using physical::unit::pi;
        return sqrt(4 * K_B * T / (pi * reduced_mass));
    }
    
    /** Calculate the standard deviation of relative velocity for a set of
     * thermalized particles at a given temperature and reduced mass.
     *
     * @param T
     *     Temperature of assumed ensemble.
     * @param reduced_mass
     *     Reduced mass of particles in assumed ensemble.
     * */
    inline double stddev_v_rel(const double & T, const double & reduced_mass) {
        using physical::constant::K_B;
        using physical::unit::pi;
        return sqrt(1.5 * K_B * T / reduced_mass);
    }

}}/* namespace particldb::interaction */
#endif // PARTICLEDB_INTERACTION_V_REL_FNC_H
