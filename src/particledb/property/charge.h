
#ifndef particledb_property_charge_h
#define particledb_property_charge_h

#include <particledb/property/Generic.h>

namespace particledb {
  namespace property {

    namespace prop_id {
      struct charge { static const char * label; };
      const char * charge::label = "charge";
    }

    /** Struct defs for charge property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<int, prop_id::charge> charge_t;
    struct charge : charge_t {
      typedef charge_t super;

      charge(const charge_t::value_type & c = charge_t().value) : charge_t(c) {}
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_charge_h
