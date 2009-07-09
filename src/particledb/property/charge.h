
#ifndef particledb_property_charge_h
#define particledb_property_charge_h

#include <particledb/property/Generic.h>

namespace particledb {
  namespace property {

    namespace label {
      struct charge { static const char * val; };
      const char * charge::val = "charge";
    }

    /** Struct defs for charge property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<int, label::charge> charge_t;
    struct charge : charge_t { typedef charge_t super; };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_charge_h
