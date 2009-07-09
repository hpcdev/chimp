
#ifndef particledb_property_Comparator_h
#define particledb_property_Comparator_h

#include <particledb/property/mass.h>
#include <particledb/property/name.h>

namespace particledb {
  namespace property {

    /** Comparator used to sort particles first by mass and then by name. */
    struct Comparator {
      template < typename Properties >
      bool operator() ( const Properties & lhs, const Properties & rhs ) {
        return lhs.mass::value < rhs.mass::value ||
               ( lhs.mass::value == rhs.mass::value &&
                 lhs.name::value < rhs.name::value );
      }
    };

  }/* namespace particledb::property */
}/* namespace particledb */

#endif // particledb_property_Comparator_h
