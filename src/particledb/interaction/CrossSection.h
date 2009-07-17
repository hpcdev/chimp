
#ifndef particledb_interaction_CrossSection_h
#define particledb_interaction_CrossSection_h

#include <particledb/interaction/v_rel_fnc.h>

#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <cmath>
#include <vector>


namespace particledb {
  namespace xml = olson_tools::xml;

  namespace interaction {

    /** Interface definition for cross section functor classes. */
    struct CrossSection {

      /* STATIC STORAGE */
      /** The factor to determine the assumed maximum relative speed of an
       * ensemble of particles based upon the standard deviation of the
       * velocity distribution.  For example, if
       * <code>MAX_SPEED_FACTOR = 3</code>, more than 9?% of all particles
       * will have a velocity lower than MAX_SPEED_FACTOR*stddev_v_rel.
       * FIXME:  calculate this number again and put it here.
       * */
      static const double MAX_SPEED_FACTOR = 3.0;



      /* MEMBER STORAGE */
      /** Reduced mass for the input particles. */
      double reduced_mass;



      /* MEMBER FUNCTIONS */
      /** Constructor sets reduced mass to 0.0 by default. */
      CrossSection(const double & mu = 0.0) : reduced_mass(mu) {}

      /** Virtual NO-OP destructor. */
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

      /** Clone the CrossSection class. */
      virtual CrossSection * new_load( xml::Context & x,
                                       const double & mu ) const = 0;

      /** Find the local maximum of cross-section*velocity assuming an
       * ensemble of particles with a given temperature.
       * */
      double find_max_sigma_v_rel_from_stddev_v(const double & stddev_v) const {
        return find_max_sigma_v_rel(MAX_SPEED_FACTOR * stddev_v);
      }

      /** Find the local maximum of cross-section*velocity assuming an
       * ensemble of particles with a given temperature.
       * */
      double find_max_sigma_v_rel_from_T(const double & T) const {
        return find_max_sigma_v_rel_from_stddev_v(stddev_v_rel(T, reduced_mass));
      }

      /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
      double effective_radius(const double & v_relative) const {
        using physical::constant::si::pi;
        return /*size * */ 0.5 * sqrt(cross_section(v_relative)/ pi);
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_CrossSection_h
