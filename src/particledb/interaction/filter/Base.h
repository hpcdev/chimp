
#ifndef particledb_interaction_filter_Base_h
#define particledb_interaction_filter_Base_h

#include <olson-tools/xml/XMLDoc.h>

#include <string>

namespace particledb {
  namespace interaction {

    namespace filter {
      namespace xml = olson_tools::xml;

      typedef xml::XMLContext::set set;

      class Base {
        /* MEMBER STORAGE */
      protected:
        /** xpath query to validate a particular context. 
         * NOTE:  this query should be written to return the original context of
         * the interaction. */
        std::string xpath_query;


        /* MEMBER FUNCTIONS */
      public:
        /** Virtual NO-OP destructor. */
        virtual ~Base() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          std::string query = xpath_query + "/Eq";
          set retval;
          for ( set::const_iterator i = in.begin(); i != in.end(); ++i ) {
            if ( i->eval(query).size() > 0 )
              retval.insert(*i);
          }
          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Base_h
