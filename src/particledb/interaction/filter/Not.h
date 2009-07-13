
#ifndef particledb_interaction_filter_Not_h
#define particledb_interaction_filter_Not_h

#include <particledb/interaction/filter/Base.h>

#include <algorithm>

namespace particledb {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;

      struct Not : filter::Base {
        /* MEMBER STORAGE */
        shared_ptr<filter::Base> f;
        set not_complement;

        /* MEMBER FUNCTIONS */
        /** Constructor */
        Not( const shared_ptr<filter::Base> & f,
             const set & not_complement = set() )
          : f(f), not_complement(not_complement) { }

        /** Virtual NO-OP destructor. */
        virtual ~Base() {}
        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set fset = f->filter(in);

          set retval;
          std::set_union( fset.begin(), fset.end(),
                          not_complement.begin(), not_complement.end(),
                          inserter(retval, retval.begin()) );
          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Not_h
