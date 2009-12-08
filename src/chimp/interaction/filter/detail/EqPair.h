
#ifndef chimp_interaction_filter_detail_EqPair_h
#define chimp_interaction_filter_detail_EqPair_h

#include <string>
#include <map>

namespace chimp {
  namespace interaction {
    namespace filter {
      namespace detail {

        struct EqPair : std::pair< const xml::Context *, const xml::Context *> {
          typedef std::pair< const xml::Context *,
                             const xml::Context *> super;

          EqPair( const xml::Context * matched = NULL,
                  const xml::Context * unmatched = NULL )
            : super(matched, unmatched) { }
        };

        typedef std::map< std::string, EqPair> EqMap;

      }/* namespace chimp::interaction::filter::detail */
    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_detail_EqPair_h
