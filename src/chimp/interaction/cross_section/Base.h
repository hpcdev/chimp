/** \file
 * Base cross section provider class.
 * */

#ifndef chimp_interaction_cross_section_Base_h
#define chimp_interaction_cross_section_Base_h

#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <cmath>
#include <vector>


namespace chimp {
  template < typename T > class RuntimeDB;

  namespace xml = olson_tools::xml;

  namespace interaction {

    /* forward declaration of Equation class. */
    template < typename T > class Equation;

    namespace cross_section {

      /** Interface definition for cross section functor classes. */
      template < typename options >
      struct Base {
        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~Base() {}

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

        /** Load a new instance of cross_section::Base. */
        virtual Base * new_load( const xml::Context & x,
                                 const interaction::Equation<options> & eq,
                                 const RuntimeDB<options> & db ) const = 0;

        /** Obtain the label of the model. */
        virtual std::string getLabel() const = 0;

      };


      /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
      template < typename options >
      double inline effectiveRadius( const Base<options> & cs,
                                     const double & v_relative ) {
        using physical::constant::si::pi;
        return /*size * */ 0.5 * sqrt( cs(v_relative) / pi );
      }

    }/* namespace particldb::interaction::cross_section */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_cross_section_Base_h
