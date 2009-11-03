/** \file
 * Implementation of VSS helper functions.
 */

#include <chimp/interaction/model/detail/vss_helpers.h>

#include <olson-tools/xml/physical_parse.h>

#include <physical/runtime.h>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace detail {

        /** load a new instance of the Interaction. */
        double loadVSSParamInv( const xml::Context & x ) {
          using runtime::physical::Quantity;
          return x.query<Quantity>("vss_param_inv")
                  .assertUnitless().getCoeff<double>();
        }

      } /* namespace chimp::interaction::model::detail */
    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

