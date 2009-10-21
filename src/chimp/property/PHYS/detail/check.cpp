#include <chimp/property/PHYS/detail/check.h>

#include <olson-tools/xml/physical_parse.h>

namespace chimp {
  namespace property {
    namespace PHYS {
      namespace detail {

        void parse_item(check<double,MASS> & out, const xml::Context & x) {
          using runtime::physical::Quantity;
          using runtime::physical::unit::kg;
          x.parse<Quantity>().assertMatch(kg).getCoeff(out.value);
        }

        void parse_item(check<double,LENGTH> & out, const xml::Context & x) {
          using runtime::physical::Quantity;
          using runtime::physical::unit::m;
          x.parse<Quantity>().assertMatch(m).getCoeff(out.value);
        }

        void parse_item(check<double,CHARGE> & out, const xml::Context & x) {
          using runtime::physical::Quantity;
          using runtime::physical::unit::C;
          x.parse<Quantity>().assertMatch(C).getCoeff(out.value);
        }

      } /* namespace chimp::property::PHYS::detail */
    } /* namespace chimp::property::PHYS */
  }/* namespace chimp::property */
}/* namespace chimp */
