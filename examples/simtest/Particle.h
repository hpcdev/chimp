
#ifndef chimp_examples_simtest_Particle_h
#define chimp_examples_simtest_Particle_h

#include <chimp/property/mass.h>

#include <olson-tools/Vector.h>
#include <olson-tools/distribution/Uniform.h>
#include <olson-tools/distribution/Gaussian.h>
#include <olson-tools/distribution/Inverter.h>

#include <vector>
#include <cstdlib>

namespace simtest {

  using olson_tools::Vector;
  using olson_tools::V3;

  /** This is the particle type that we will be using.  */
  struct Particle {
    Vector<double, 3> x;/**< position */
    Vector<double, 3> v;/**< velocity */
    int type;                        /**< species */

    /* default construtor sets all to zero. */
    Particle( const Vector<double,3> & x = 0.0,
              const Vector<double,3> & v = 0.0,
              const int & type = 0 )
      : x(x), v(v), type(type) { }
  };


  std::ostream & operator<< (std::ostream & out, const Particle & p) {
    return out << "{x: (" << p.x[0] << ", " << p.x[1] << ", " << p.x[2] << "), "
                   "v: (" << p.v[0] << ", " << p.v[1] << ", " << p.v[2] << "), "
                   "t: " << p.type   << '}';
  }

  /** Create a bunch of particles from a set of given species and with the given
   * temperature.
   * @tparam RnDB        The type of runtime database. 
   *
   * @param pv  Particle Vector.
   * @param n_particles  Number of particles to create.
   * @param temperature  Temperature of the set of particles.  
   * @param db           The runtime particle database from which to get mass
   *                     and species.
   */
  template < typename RnDB >
  void createRandomParticles( std::vector<Particle> & pv,
                              const int & n_particles,
                              const double & temperature,
                              const RnDB & db ) {
    namespace dist = olson_tools::distribution;

    /** velocity distributions per species. */
    std::vector< dist::Inverter > velocity;

    { /* first set up the initial velocity distributions. */
      typedef DB::PropertiesVector::const_iterator PIter;
      PIter end = db.getProps().end();
      for ( PIter i  = db.getProps().begin(); i != end; ++i ) {
        /* set the velocity distribution for this particle type using the global
         * temperature. */
        using chimp::property::mass;
        double beta = 0.5 * i->mass::value / (K_B * temperature);
        double sigma = std::sqrt( 0.5 / beta );
        using dist::Inverter;
        using dist::Gaussian;
        velocity.push_back( Inverter( Gaussian(beta), -4*sigma, 4*sigma ) );
      }
    }


    /** position distribution (shared by all species). */
    dist::Inverter position( dist::Uniform(), -1, 1 );

    /** random species. */
    dist::Inverter species( dist::Uniform(), 0, 0.9999999*db.getProps.size() );

    pv.resize(0);              /* start with empty array */
    pv.reserve( n_particles ); /* allocate right away */
    for ( int i = 0; i < n_particles; ++i ) {
      /* pick the species first. */
      int S = static_cast<int>( species() );

      /* pick position and velocity. */
      Vector<double,3> x = V3(
                          position(),
                          position(),
                          position()
                       ),
                       v = V3(
                          velocity[ S ](),
                          velocity[ S ](),
                          velocity[ S ]()
                       );

      /* Assign the result. */
      pv.push_back( Particle(x,v,S) );
    }
  }

}/* namespace simtest */

#endif // chimp_examples_simtest_Particle_h
