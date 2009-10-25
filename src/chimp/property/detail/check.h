
#ifndef chimp_property_detail_check_h
#define chimp_property_detail_check_h

#include <olson-tools/xml/Doc.h>
//#include <olson-tools/xml/physical_parse.h>

#include <physical/runtime.h>

#include <string>

namespace chimp {
  namespace property {
    namespace detail {

      /** Non-dimension used as default arg of Generic class.  This is used when
       * a physical dimension is not specified. */
      struct NullDimension {};


      using runtime::physical::system::si;
      namespace dim = runtime::physical::dimension;

      namespace xml = olson_tools::xml;

      template < typename T, typename dimT >
      struct check {
        check(const T& t = T()) : value(t) {}
        T value;
      };

      ///** Generic implementation of parse_item< check<?,?> >.  Unfortunately,
      // * this compiles way to slow as of yet. */
      //template < typename T, typename dimT >
      //static void parse_item( check<T,dimT> & out,
      //                        const xml::Context & x ) {
      //  using runtime::physical::Quantity;
      //  x.parse<Quantity>().assertMatch( dimT::value ).getCoeff(out.value);
      //}

      template < typename T >
      static void parse_item( check<T,NullDimension> & out,
                              const xml::Context & x ) {
        out.value = x.parse<T>();
      }

      /** parse_item implementation used for reading in physical::Quantity
       * values which are checked to ensure mass units.  This code is called by
       * the xml::Context::query and xml::Context::parse functions and does not
       * need to be called by a user.
       */
      extern void parse_item( check< double, dim::mass<si> > & out,
                              const xml::Context & x );

      /** parse_item implementation used for reading in physical::Quantity
       * values which are checked to ensure length units.  This code is called by
       * the xml::Context::query and xml::Context::parse functions and does not
       * need to be called by a user.
       */
      extern void parse_item( check< double, dim::length<si> > & out,
                              const xml::Context & x );

      /** parse_item implementation used for reading in physical::Quantity
       * values which are checked to ensure length units.  This code is called by
       * the xml::Context::query and xml::Context::parse functions and does not
       * need to be called by a user.
       */
      extern void parse_item( check< double, dim::charge<si> > & out,
                              const xml::Context & x );

    }/* namespace chimp::property::detail */
  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_detail_check_h
