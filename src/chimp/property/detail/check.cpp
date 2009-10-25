#include <chimp/property/detail/check.h>

#include <olson-tools/xml/physical_parse.h>

namespace chimp {
  namespace property {
    namespace detail {

      /** Generic implementation of parse_item< check<?,?> >.  Unfortunately,
       * this compiles way to slow as of yet. */
      template < typename T, typename dimT >
      static void parse_item( check<T,dimT> & out,
                              const xml::Context & x ) {
        using runtime::physical::Quantity;
        x.parse<Quantity>().assertMatch( dimT::value ).getCoeff(out.value);
      }

      #if 0
      extern "C++" {
        /* Trying out this method of forcing the compiler to keep the parse_item
         * implementations around.  If Boost.Spirit weren't so annoyingly slow
         * to compile, then I'd just use a header implementation.  As it is, I'm
         * trying to compartmentalize the areas where Boost.Spirit needs to be
         * compiled.  */

        //trying out the generic implementation for right now.  This might end up
        //being a bad idea when I try other compilers.

        void (*pi_mass)(check< double, dim::mass<si> > &, const xml::Context &)
          = parse_item< double, dim::mass<si> >;

        void (*pi_len)(check< double, dim::length<si> > &, const xml::Context &)
          = parse_item< double, dim::length<si> >;

        void (*pi_ch)(check< double, dim::charge<si> > &, const xml::Context &)
          = parse_item< double, dim::charge<si> >;
      }

      #else
      void parse_item( check<double, dim::mass<si> > & out,
                       const xml::Context & x ) {
        using runtime::physical::Quantity;
        x.parse<Quantity>().assertMatch( dim::mass<si>::value ).getCoeff(out.value);
      }

      void parse_item( check<double, dim::length<si> > & out,
                       const xml::Context & x ) {
        using runtime::physical::Quantity;
        x.parse<Quantity>().assertMatch(dim::length<si>::value).getCoeff(out.value);
      }

      void parse_item( check<double, dim::charge<si> > & out,
                       const xml::Context & x ) {
        using runtime::physical::Quantity;
        x.parse<Quantity>().assertMatch(dim::charge<si>::value).getCoeff(out.value);
      }
      #endif

    } /* namespace chimp::property::detail */
  }/* namespace chimp::property */
}/* namespace chimp */
