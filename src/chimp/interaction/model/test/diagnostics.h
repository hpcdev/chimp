
#ifndef chimp_interaction_model_test_diagnostics_h
#define chimp_interaction_model_test_diagnostics_h

#include <chimp/property/mass.h>
#include <chimp/property/size.h>
#include <chimp/interaction/ParticleAccessors.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace test {
        using olson_tools::Vector;

        /** Calculate the energy of a particle. */
        template < typename Particle, typename RnDB  >
        inline double energy( const Particle & p,
                              const int & part_i,
                              const RnDB & db ) {
          using olson_tools::SQR;
          const typename RnDB::options::Properties & prop = db[part_i];
          const double & sz = prop.property::size::value;
          const double & m = prop.property::mass::value;
          return sz * 0.5 * m * SQR( velocity(p) );
        }


        /** Calculate the energy of a particle. */
        template < typename Particle, typename RnDB  >
        inline Vector<double,3> momentum( const Particle & p,
                                          const int & part_i,
                                          const RnDB & db ) {
          const typename RnDB::options::Properties & prop = db[part_i];
          const double & sz = prop.property::size::value;
          const double & m = prop.property::mass::value;
          return (sz * m) * velocity(p);
        }

      }/* namespace chimp::interaction::model::test */
    }/* namespace chimp::interaction::model */
  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_model_test_diagnostics_h
