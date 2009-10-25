/** \file
 * Definition of mass property. 
 */

#ifndef chimp_property_mass_h
#define chimp_property_mass_h

#include <chimp/property/Generic.h>

#include <physical/physical.h>

namespace chimp {
  namespace property {

    /** Namespace for the properties tag classes that provide the xpath/xml
     * labels/queries. */
    namespace tag {
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
    struct mass : Generic<double, tag::mass<>, false, dim::mass<si> > {
      /** Typedef of derivative of Generic class--required by property::Add. */
      typedef Generic< double, tag::mass<>, false, dim::mass<si> > super;

      /** Mass property constructor.  By default, this sets mass to 'zero'.  */
      mass(const super::value_type & m = super().value) : super(m) {}
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_mass_h
