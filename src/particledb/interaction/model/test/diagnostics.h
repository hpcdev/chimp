
#ifndef particledb_interaction_model_test_diagnostics_h
#define particledb_interaction_model_test_diagnostics_h

#include <particledb/property/mass.h>
#include <particledb/property/size.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>

namespace particledb {
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
          return sz * 0.5 * m * SQR(p.v);
        }


        /** Calculate the energy of a particle. */
        template < typename Particle, typename RnDB  >
        inline Vector<double,3> momentum( const Particle & p,
                                          const int & part_i,
                                          const RnDB & db ) {
          const typename RnDB::options::Properties & prop = db[part_i];
          const double & sz = prop.property::size::value;
          const double & m = prop.property::mass::value;
          return (sz * m) * p.v;
        }

      }/* namespace particledb::interaction::model::test */
    }/* namespace particledb::interaction::model */
  }/* namespace particledb::interaction */
}/* namespace particledb */

#endif // particledb_interaction_model_test_diagnostics_h
