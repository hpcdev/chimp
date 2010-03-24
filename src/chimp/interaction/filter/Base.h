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
 * Declaration of filter::Base class. 
 */

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
          for ( set::const_iterator i  = in.begin(),
                                  end  = in.end();
                                    i != end; ++i ) {
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
