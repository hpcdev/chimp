
#ifndef particledb_property_name_h
#define particledb_property_name_h

#include <particledb/property/Generic.h>

#include <string>

namespace particledb {
  namespace property {

    namespace prop_id {
      struct name { static const char * label; };
      const char * name::label = "@name";
    }

    /** Struct defs for name property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<std::string, prop_id::name, true> name_t;
    struct name : name_t {
      typedef name_t super;

      name(const name_t::value_type & n = name_t().value) : name_t(n) {}
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_name_h
