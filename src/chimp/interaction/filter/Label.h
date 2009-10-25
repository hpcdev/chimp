/** \file
 * Declaration of filter::Label class. 
 */

#ifndef chimp_interaction_filter_Label_h
#define chimp_interaction_filter_Label_h

#include <chimp/interaction/filter/Base.h>

#include <string>
#include <sstream>
#include <set>

namespace chimp {
  namespace interaction {
    namespace filter {

      /** Filters an Equation based on a set of Input/Output terms. */
      struct Label : filter::Base {
        /* MEMBER FUNCTIONS */
        /** Default constructor does not initialize the xpath filter string. */
        Label( const std::string & label ) {
          xpath_query = "self::node()[@type='" + label + "']";
        }

      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Label_h
