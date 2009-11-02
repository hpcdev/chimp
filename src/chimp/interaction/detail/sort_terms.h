
#ifndef chimp_interaction_detail_sort_terms_h
#define chimp_interaction_detail_sort_terms_h

#include <chimp/property/Comparator.h>

#include <map>

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


      template < typename RnDB >
      struct makeSortedTermMap {
        typedef std::map<
          const typename RnDB::Properties *,
          int,
          PropertyPtrComparator
        > type;
      };


      /** Load up terms and sort them according to the prescribed mass,
       * then name comparison. @see detail::PropertyPtrComparator which is
       * actuall just a proxy for chimp::property::Comparator. 
       */
      template < typename RnDB >
      int loadAndSortTerms( const xml::Context::list & xl,
                            typename makeSortedTermMap<RnDB>::type & el,
                            RnDB & db ) {
        typedef xml::Context::list::const_iterator CXIter;

        int n_particles = 0;
        for (CXIter i = xl.begin(); i != xl.end(); ++i ) {
          string particle_name = i->query<string>("P");
          int n = i->query<int>("n",1);

          n_particles += n;

          const typename RnDB::Properties * p = &( db[particle_name] );

          PIter j = el.find(p);
          if ( j != el.end() )
            j->second += n;
          else
            el.insert( std::make_pair(p, n) );
        }

        return n_particles;
      }

    }/* namespace particldb::interaction::detail */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_detail_sort_terms_h
