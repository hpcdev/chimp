
#ifndef chimp_property_Comparator_h
#define chimp_property_Comparator_h

#include <chimp/property/mass.h>
#include <chimp/property/name.h>

namespace chimp {
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

  }/* namespace chimp::property */
}/* namespace chimp */

#endif // chimp_property_Comparator_h
