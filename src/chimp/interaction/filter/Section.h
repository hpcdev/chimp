/** \file
 * Declaration of filter::Section class. 
 */

#ifndef chimp_interaction_filter_Section_h
#define chimp_interaction_filter_Section_h

#include <chimp/interaction/filter/Base.h>
#include <chimp/interaction/filter/Null.h>
#include <chimp/interaction/filter/detail/EqPair.h>

#include <boost/shared_ptr.hpp>

#include <string>

namespace chimp {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;


      /** This filter sets an order of precedence on interactions coming from
       * different sections of the database. */
      struct Section : filter::Base {
        /* TYPEDEFS */
        /** Specification values for the type of Section filter. */
        enum REQUIREMENT {
          /** Require all interactions to be from the specific section. */
          REQUIRED,
          /** Prefer all interactions to be from the specific section. */
          PREFERED
        };


        /* MEMBER STORAGE */
        /** Pointer to filter sub-tree. */
        shared_ptr<filter::Base> f;

        /** Section name to apply preference to.  This string is relative to the
         * root /ParticleDB section and can be any xpath expression that
         * leaves the context relative to the section, such as
         * '/ParticleDB/standard'.
         * [Default:  standard]
         */
        std::string section;

        /** Effect of this filter. */
        enum REQUIREMENT requirement;



        /* MEMBER FUNCTIONS */
        /** Constructor */
        Section( const std::string & section = "standard",
                 const enum REQUIREMENT & requirement = PREFERED,
                 const shared_ptr<filter::Base> & f
                   = shared_ptr<filter::Base>(new Null) )
          : f(f), section(section), requirement(requirement) { }

        /** Virtual NO-OP destructor. */
        virtual ~Section() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set fset = f->filter(in);

          detail::EqMap map;

          for ( set::iterator i = fset.begin(); i != fset.end(); ++i ) {
            /* we need to test to see if the section matches. */
            std::string estr = i->query<std::string>("Eq");

            bool match =
              i->query<std::string>(
                "/ParticleDB/" + section +
                "//Interaction[string(Eq) = '"+estr+"']/Eq",
                ""
              ) == estr ;

            if ( match )
              map[estr].first = &(*i);
            else
              map[estr].second = &(*i);
          }

          /* now insert the appropriate item. */
          set retval;
          for ( detail::EqMap::iterator i = map.begin(); i != map.end(); ++i ) {
            assert(i->second.first || i->second.second);

            if (i->second.first)
              retval.insert( *(i->second.first) );
            else if ( requirement != REQUIRED )
              retval.insert( *(i->second.second) );
          }

          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Section_h
