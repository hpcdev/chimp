
#ifndef particledb_property_mass_h
#define particledb_property_mass_h

#include <particledb/property/Generic.h>

namespace particledb {
  namespace property {

    namespace label {
      struct mass { static const char * val; };
      const char * mass::val = "mass";
    }


    /** Struct defs for mass property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<double, label::mass, false, PHYS::KG> mass_t;
    struct mass : mass_t { typedef mass_t super; };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_mass_h
