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
    static Vector<double,3> & velocity( ParticleT & p ) {
      return p.v;
    }

    /** Generic const accessor for particle velocity. */
    template < typename ParticleT >
    static const Vector<double,3> & velocity( const ParticleT & p ) {
      return p.v;
    }

    /** Generic particle velocity set function. */
    template < typename ParticleT >
    static void setVelocity( ParticleT & p, const Vector<double,3> & v ) {
      p.v = v;
    }


    /** Generic NON-const accessor for particle position. */
    template < typename ParticleT >
    static Vector<double,3> & position( ParticleT & p ) {
      return p.x;
    }

    /** Generic const accessor for particle position. */
    template < typename ParticleT >
    static const Vector<double,3> & position( const ParticleT & p ) {
      return p.x;
    }


    /** Generic NON-const accessor for particle species. */
    template < typename ParticleT >
    static int & species( ParticleT & p ) {
      return p.type;
    }

    /** Generic const accessor for particle species. */
    template < typename ParticleT >
    static const int & species( const ParticleT & p ) {
      return p.type;
    }

    /** Generic accessor for particle weight:  defaults to just returning 1. */
    template < typename ParticleT >
    static double weight( const ParticleT & p ) {
      return 1;
    }

  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_ParticleAccessors_h
