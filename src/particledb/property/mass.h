
#ifndef particledb_property_mass_h
#define particledb_property_mass_h

#include <particledb/property/Generic.h>

namespace particledb {
  namespace property {

    namespace prop_id {
      template < unsigned int i = 0 >
      struct mass { static const char * label; };

      template < unsigned int i >
      const char * mass<i>::label = "mass";
    }

    /** Struct defs for mass property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<double, prop_id::mass<>, false, PHYS::KG> mass_t;
    struct mass : mass_t {
      typedef mass_t super;

      mass(const mass_t::value_type & m = mass_t().value) : mass_t(m) {}
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_mass_h
