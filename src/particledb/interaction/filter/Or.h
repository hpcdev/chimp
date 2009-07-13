
#ifndef particledb_interaction_filter_Or_h
#define particledb_interaction_filter_Or_h

#include <particledb/interaction/filter/Base.h>

#include <boost/shared_ptr.hpp>

#include <algorithm>

namespace particledb {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;

      struct Or : filter::Base {
        /* MEMBER STORAGE */
        shared_ptr<filter::Base> l, r;

        /* MEMBER FUNCTIONS */
        /** Constructor */
        Or( const shared_ptr<filter::Base> & l,
            const shared_ptr<filter::Base> & r ) : l(l), r(r) { }

        /** Virtual NO-OP destructor. */
        virtual ~Or() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set lset = l->filter(in);
          set rset = r->filter(in);

          set retval;
          std::set_union( lset.begin(), lset.end(),
                          rset.begin(), rset.end(),
                          inserter(retval, retval.begin()) );
          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Or_h
