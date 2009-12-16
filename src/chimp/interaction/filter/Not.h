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
 * Declaration of filter::Not class. 
 */

#ifndef chimp_interaction_filter_Not_h
#define chimp_interaction_filter_Not_h

#include <chimp/interaction/filter/Base.h>

#include <boost/shared_ptr.hpp>

#include <algorithm>

namespace chimp {
  namespace interaction {
    namespace filter {

      using boost::shared_ptr;

      /** Filters by performing a set difference (pos - neg). */
      struct Not : filter::Base {
        /* MEMBER STORAGE */
        shared_ptr<filter::Base> pos, neg;

        /* MEMBER FUNCTIONS */
        /** Constructor */
        Not( const shared_ptr<filter::Base> & pos,
             const shared_ptr<filter::Base> & neg )
          : pos(pos), neg(neg) { }

        /** Virtual NO-OP destructor. */
        virtual ~Not() {}

        /** Virtual filter operation. */
        virtual set filter(const set & in) {
          set pset = pos->filter(in);
          set nset = neg->filter(in);

          set retval;
          std::set_difference( pset.begin(), pset.end(),
                               nset.begin(), nset.end(),
                               inserter(retval, retval.begin()) );
          return retval;
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Not_h
