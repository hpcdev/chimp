/** \file
 * Base cross section provider class.
 * */

#ifndef chimp_interaction_CrossSection_h
#define chimp_interaction_CrossSection_h

#include <chimp/interaction/v_rel_fnc.h>

#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <cmath>
#include <vector>


namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {

    /** Interface definition for cross section functor classes. */
    struct CrossSection {
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
      virtual double operator() (const double & v_relative) const  = 0;

      /** Find the local maximum of cross-section*velocity (within a given
       * range of velocity space.  The actual implementation of this function is
       * done by the specific cross section model. 
       * */
      virtual double findMaxSigmaVProduct(const double & v_rel_max) const = 0;

      /** Clone the CrossSection class. */
      virtual CrossSection * new_load( xml::Context & x,
                                       const double & mu ) const = 0;
    };


    /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
    double inline effectiveRadius( const CrossSection & cs,
                                   const double & v_relative ) {
      using physical::constant::si::pi;
      return /*size * */ 0.5 * sqrt( cs(v_relative) / pi );
    }

  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_CrossSection_h
