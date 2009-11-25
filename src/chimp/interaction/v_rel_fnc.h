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
     * */
    inline double calcStdVFromT(const double & T, const double & reduced_mass) {
      using physical::constant::si::K_B;
      using physical::constant::si::pi;
      return std::sqrt(1.5 * K_B * T / reduced_mass);
    }



    /** The factor to determine the assumed maximum relative speed of an
     * ensemble of particles based upon the standard deviation of the
     * velocity distribution.  For example, if
     * <code>MAX_SPEED_FACTOR = 3</code>, more than 9?% of all particles
     * will have a velocity lower than MAX_SPEED_FACTOR*stddev_v_rel.
     * FIXME:  calculate this number again and put it here.
     * */
    static const double MAX_SPEED_FACTOR = 3.0;

    /** Estimate the local maximum of cross-section*velocity assuming an
     * ensemble of particles with a given speed standard deviation.
     * */
    inline double estMaxVFromStdV(const double & stddev_v) {
      return MAX_SPEED_FACTOR * stddev_v;
    }

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // chimp_interaction_v_rel_fnc_h
