/** \file
 * Declaration of filter::Elastic class. 
 */

#ifndef chimp_interaction_filter_Elastic_h
#define chimp_interaction_filter_Elastic_h

#include <chimp/interaction/filter/Base.h>

#include <string>

namespace chimp {
  namespace interaction {
    namespace filter {

      /** Filters out all inelastic interactions (only elastic interactions make
       * it through). */
      struct Elastic : Base {
        /* TYPEDEFS */
        /** Simply a place to put the elastic predicate so that I don't have to
         * put it in a .cpp file that gets compiled into a .o file, just to have
         * the string.  I guess I <i>could</i> use a <code>\#define</code>
         * instead. */
        template < unsigned int i = 0 >
        struct elastic_predicate {
        /** query statement for elastic interactions. */
          static const char * xpath_query;
        };

        /* MEMBER FUNCTIONS */
        Elastic() {
          xpath_query = elastic_predicate<>::xpath_query;
        }
      };

      template < unsigned int i >
      const char * Elastic::elastic_predicate<i>::xpath_query
        = "Eq[string(In) = string(Out)]/..";

      /** Determine whether the equation that is assumed to be represented at
       * the given XML context is an elastic interaction.
       *
       * @param x
       *    The xml::Context of an Interaction.
       */
      inline bool isElastic( const xml::Context & x ) {
        using std::string;
        string query = string(Elastic::elastic_predicate<>::xpath_query) + "/Eq";

        if ( x.eval(query).size() > 0 )
          return true;
        else
          return false;
      }

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Elastic_h
