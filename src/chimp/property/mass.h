
#ifndef chimp_property_mass_h
#define chimp_property_mass_h

#include <chimp/property/Generic.h>

namespace chimp {
  namespace property {

    /** Namespace for the properties tag classes that provide the xpath/xml
     * labels/queries. */
    namespace prop_tag {
      /** The tag class for mass. */
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
    typedef Generic<double, prop_tag::mass<>, false, PHYS::KG> mass_t;
    struct mass : mass_t {
      /** Typedef of derivative of Generic class--required by property::Add. */
      typedef mass_t super;

      /** Mass property constructor.  By default, this sets mass to 'zero'.  */
      mass(const mass_t::value_type & m = mass_t().value) : mass_t(m) {}
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_mass_h
