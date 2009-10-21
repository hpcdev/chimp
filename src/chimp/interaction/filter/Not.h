
#ifndef chimp_interaction_filter_Not_h
#define chimp_interaction_filter_Not_h

#include <chimp/interaction/filter/Base.h>

#include <boost/shared_ptr.hpp>

#include <algorithm>

namespace chimp {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;

      /** Filters by performing a set difference (pos - neg). */
      struct Not : filter::Base {
        /* MEMBER STORAGE */
        shared_ptr<filter::Base> pos, neg;

        /* MEMBER FUNCTIONS */
        /** Constructor */
        Not( const shared_ptr<filter::Base> & pos,
             const shared_ptr<filter::Base> & neg )
          : pos(pos), neg(neg) { }

        /** Virtual NO-OP destructor. */
        virtual ~Not() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set pset = pos->filter(in);
          set nset = neg->filter(in);

          set retval;
          std::set_difference( pset.begin(), pset.end(),
                               nset.begin(), nset.end(),
                               inserter(retval, retval.begin()) );
          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Not_h
