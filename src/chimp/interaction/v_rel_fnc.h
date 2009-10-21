#ifndef chimp_interaction_v_rel_fnc_h
#define chimp_interaction_v_rel_fnc_h

#include <physical/physical.h>

#include <cmath>

namespace chimp {
  namespace interaction {

    /** Calculate the average relative velocity for a set of thermalized
     * particles at a given temperature and reduced mass.
     *
     * @param T
     *     Temperature of assumed ensemble.
     * @param reduced_mass
     *     Reduced mass of particles in assumed ensemble.
     * FIXME:  Check the coefficients inside the sqrt.
     * */
    inline double avg_v_rel(const double & T, const double & reduced_mass) {
      using physical::constant::si::K_B;
      using physical::constant::si::pi;
      return std::sqrt(4 * K_B * T / (pi * reduced_mass));
    }
    
    /** Calculate the standard deviation of relative velocity for a set of
     * thermalized particles at a given temperature and reduced mass.
     *
     * @param T
     *     Temperature of assumed ensemble.
     * @param reduced_mass
     *     Reduced mass of particles in assumed ensemble.
     * FIXME:  Check the coefficients inside the sqrt.
     * */
    inline double stddev_v_rel(const double & T, const double & reduced_mass) {
      using physical::constant::si::K_B;
      using physical::constant::si::pi;
      return std::sqrt(1.5 * K_B * T / reduced_mass);
    }

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // chimp_interaction_v_rel_fnc_h
