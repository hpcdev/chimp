/** \file
 * Helper functions for loading information useful for the VSSElastic model.
 */

#ifndef chimp_interaction_model_detail_vss_helpers_h
#define chimp_interaction_model_detail_vss_helpers_h

#include <olson-tools/xml/Doc.h>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace model {
      namespace detail {

        /** load a new instance of the Interaction. */
        double loadVSSParamInv( const xml::Context & x );

      } /* namespace chimp::interaction::model::detail */
    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_detail_vss_helpers_h
