
#ifndef chimp_property_PHYS_check_h
#define chimp_property_PHYS_check_h

#include <chimp/property/PHYS/ID.h>

#include <olson-tools/xml/Doc.h>

#include <string>

namespace chimp {
  namespace property {
    namespace PHYS {
      namespace detail {

        namespace xml = olson_tools::xml;

        template <class T, enum ID phys_id>
        struct check {
          check(const T& t = T()) : value(t) {}
          T value;
        };

        template<class T, enum ID phys_id>
        static void parse_item( check<T,phys_id> & out,
                                const xml::Context & x ) {
          out.value = x.parse<T>();
        }

        /** parse_item implementation used for reading in physical::Quantity
         * values which are checked to ensure mass units.  This code is called by
         * the xml::Context::query and xml::Context::parse functions and does not
         * need to be called by a user.
         * TODO:  put this and the check<T,ID> class into a detail namespace.
         */
        extern void parse_item( check<double,KG> & out,
                                const xml::Context & x );

        /** parse_item implementation used for reading in physical::Quantity
         * values which are checked to ensure length units.  This code is called by
         * the xml::Context::query and xml::Context::parse functions and does not
         * need to be called by a user.
         * TODO:  put this and the check<T,ID> class into a detail namespace.
         */
        extern void parse_item( check<double,M>  & out,
                                const xml::Context & x );

      }/* namespace chimp::property::PHYS::detail */
    }/* namespace chimp::property::PHYS */
  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_PHYS_check_h
