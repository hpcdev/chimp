/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Copyright (C) 2006, 2008 Spencer E. Olson                                   *
 *                                                                             *
 * This file is part of CHIMP                                                  *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


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
          PREFERRED
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
                 const enum REQUIREMENT & requirement = PREFERRED,
                 const shared_ptr<filter::Base> & f
                   = shared_ptr<filter::Base>(new Null) )
          : f(f), section(section), requirement(requirement) { }

        /** Virtual NO-OP destructor. */
        virtual ~Section() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set fset = f->filter(in);

          detail::EqMap map;

          for ( set::iterator i  = fset.begin(),
                            end  = fset.end();
                              i != end; ++i ) {
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
          for ( detail::EqMap::iterator i  = map.begin(),
                                      end  = map.end();
                                        i != end; ++i ) {
            assert(i->second.first || i->second.second);

            if (i->second.first)
              retval.insert( *(i->second.first) );
            else if ( requirement != REQUIRED )
              retval.insert( *(i->second.second) );
          }

          return retval;
        }
      };


      namespace loader {
        struct Section : filter::loader::Base {
          typedef shared_ptr<filter::Base> SHB;
          virtual ~Section() { }

          virtual SHB load( const xml::Context & x ) const {
            std::string section = x.query< std::string >("@name", "standard");
            std::string prefer
              = x.query< std::string >("@preference", "preferred");

            xml::Context::list x_list = x.eval("child::node()");// get all children

            if ( x_list.size() == 0u )
              return SHB( new filter::Section( section, prefer ) );
            else if ( x_list.size() > 1u )
              throw xml::error(
                "<equation-filter> should only have one direct child node"
              );

            SHB f = filter::loader::list[ x_list.front().name() ]->load(x);

            return SHB( new filter::Section( section, prefer, f ) );
          }
        };
      }/* namespace chimp::interaction::filter::loader */

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Section_h
