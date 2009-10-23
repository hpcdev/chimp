
#ifndef chimp_interaction_detail_PropertyPtrComparator_h
#define chimp_interaction_detail_PropertyPtrComparator_h

#include <chimp/property/Comparator.h>

namespace chimp {
  namespace interaction {
    namespace detail {

      /** Comparator used to sort particles first by mass and then by name. */
      struct PropertyPtrComparator : chimp::property::Comparator {
        typedef chimp::property::Comparator super;

        template < typename Properties >
        bool operator() ( const Properties * lhs, const Properties * rhs ) {
          return super::operator()(*lhs,*rhs);
        }
      };

    }/* namespace particldb::interaction::detail */
  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // chimp_interaction_detail_PropertyPtrComparator_h
