/** \file
 * Definition of name property. 
 */

#ifndef chimp_property_name_h
#define chimp_property_name_h

#include <chimp/property/Generic.h>

#include <string>

namespace chimp {
  namespace property {

    /** Namespace for the properties tag classes that provide the xpath/xml
     * labels/queries. */
    namespace tag {
      /** The tag class for name. */
      template < unsigned int i = 0 >
      struct name { static const char * label; };

      template < unsigned int i >
      const char * name<i>::label = "@name";
    }

    /** Struct defs for name property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    struct name : Generic<std::string, tag::name<>, true> {
      /** Typedef of derivative of Generic class--required by property::Add. */
      typedef Generic<std::string, tag::name<>, true> super;

      /** Name property constructor.  By default, this sets name to '' (empty
       * string).  */
      name(const super::value_type & n = super().value) : super(n) {}
      /** Name property constructor where the name is taken from the given
       * <code>char*</code>. */
      name(const char * n) : super(n) {}
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_name_h
