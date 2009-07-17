
#ifndef particledb_interaction_filter_Elastic_h
#define particledb_interaction_filter_Elastic_h

#include <particledb/interaction/filter/Base.h>

#include <string>

namespace particledb {
  namespace interaction {
    namespace filter {

      namespace detail {
        template < unsigned int i = 0 >
        struct elastic_parms {
        /** query statement for elastic interactions. */
          static const char * xpath_query;
        };

        template < unsigned int i >
        const char * elastic_parms<i>::xpath_query = "Eq[string(In) = string(Out)]/..";
      }

      /** Determine whether the equation that is assumed to be represented at
       * the given XML context is an elastic interaction. */
      inline bool is_elastic( const xml::Context & x ) {
        using std::string;
        string query = string(detail::elastic_parms<>::xpath_query) + "/Eq";

        if ( x.eval(query).size() > 0 )
          return true;
        else
          return false;
      }

      /** Filters out all inelastic interactions (only elastic interactions make
       * it through). */
      struct Elastic : Base {
        /* MEMBER FUNCTIONS */
        Elastic() {
          xpath_query = detail::elastic_parms<>::xpath_query;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Elastic_h
