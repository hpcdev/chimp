/** \file
 * Declaration of generic particle accessor functions such as velocity,
 * position, type, and species.
 */

#ifndef chimp_interaction_ParticleAccessors_h
#define chimp_interaction_ParticleAccessors_h


#include <olson-tools/Vector.h>

namespace chimp {
  namespace interaction {

    using olson_tools::Vector;

    /** Generic NON-const accessor for particle velocity. */
    template < typename ParticleT >
    Vector<double,3> & velocity( ParticleT & p ) {
      return p.v;
    }

    /** Generic const accessor for particle velocity. */
    template < typename ParticleT >
    const Vector<double,3> & velocity( const ParticleT & p ) {
      return p.v;
    }


    /** Generic NON-const accessor for particle position. */
    template < typename ParticleT >
    Vector<double,3> & position( ParticleT & p ) {
      return p.x;
    }

    /** Generic const accessor for particle position. */
    template < typename ParticleT >
    const Vector<double,3> & position( const ParticleT & p ) {
      return p.x;
    }


    /** Generic NON-const accessor for particle species. */
    template < typename ParticleT >
    int & species( ParticleT & p ) {
      return p.type;
    }

    /** Generic const accessor for particle species. */
    template < typename ParticleT >
    const int & species( const ParticleT & p ) {
      return p.type;
    }

  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_ParticleAccessors_h
