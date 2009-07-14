
#ifndef particledb_interaction_filter_Not_h
#define particledb_interaction_filter_Not_h

#include <particledb/interaction/filter/Base.h>

#include <boost/shared_ptr.hpp>

#include <algorithm>

namespace particledb {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;

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

#endif // particledb_interaction_filter_Not_h
