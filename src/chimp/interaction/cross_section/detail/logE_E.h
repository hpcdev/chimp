
#ifndef chimp_interaction_cross_section_detail_logE_E_h
#define chimp_interaction_cross_section_detail_logE_E_h

#include <cmath>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace cross_section {
      namespace detail {

        /** Calculate the log(E)/E exponential fall-off of the cross section. */
        inline double g(const double & x) {
          register double s = x + M_E;
          return M_E * std::log(s)/s;
        }

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_detail_logE_E_h
