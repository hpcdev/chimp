
#ifndef chimp_property_charge_h
#define chimp_property_charge_h

#include <chimp/property/Generic.h>

namespace chimp {
  namespace property {

    namespace prop_tag {
      template < unsigned int i = 0>
      struct charge { static const char * label; };

      template < unsigned int i >
      const char * charge<i>::label = "charge";
    }

    /** Struct defs for charge property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    typedef Generic<int, prop_tag::charge<> > charge_t;
    struct charge : charge_t {
      typedef charge_t super;

      charge(const charge_t::value_type & c = charge_t().value) : charge_t(c) {}
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_charge_h
