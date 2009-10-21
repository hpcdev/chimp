
#ifndef chimp_interaction_filter_Base_h
#define chimp_interaction_filter_Base_h

#include <olson-tools/xml/Doc.h>

#include <string>

namespace chimp {
  namespace interaction {

    namespace filter {
      namespace xml = olson_tools::xml;

      typedef xml::Context::set set;

      /** Filter interface definition class.  All other filters must inherit
       * from this class.  Simple filters (those that can be accomplished by
       * filtering via an XPath query) can be implemented easily by 1)
       * inheriting from this class, and 2) setting the protected xpath_query
       * member variable to something appropriate.  See the Elastic filter for a
       * prime example of this.
       */
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

#endif // chimp_interaction_filter_Base_h
