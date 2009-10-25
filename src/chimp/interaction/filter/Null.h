/** \file
 * Declaration of filter::Null class. 
 */

#ifndef chimp_interaction_filter_Null_h
#define chimp_interaction_filter_Null_h

#include <chimp/interaction/filter/Base.h>

namespace chimp {
  namespace interaction {

    namespace filter {

      /** This filter is really a NO-OP since it does not perform any type of
       * filtering at all. */
      struct Null : filter::Base {
        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~Null() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          return in;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Null_h
