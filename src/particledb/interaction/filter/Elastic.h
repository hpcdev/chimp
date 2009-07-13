
#ifndef particledb_interaction_filter_Elastic_h
#define particledb_interaction_filter_Elastic_h

#include <particledb/interaction/filter/Base.h>

#include <string>

namespace particledb {
  namespace interaction {
    namespace filter {

      /** query statement for elastic interactions. */
      const char * elastic_xpath_query = "Eq[string(In) = string(Out)]/..";

      inline bool is_elastic( const xml::XMLContext & x ) {
        std::string query = std::string(elastic_xpath_query) + "/Eq";
        if ( x.eval(query).size() > 0 )
          return true;
        else
          return false;
      }

      struct Elastic : Base {
        /* MEMBER FUNCTIONS */
        Elastic() {
          xpath_query = elastic_xpath_query;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Elastic_h
