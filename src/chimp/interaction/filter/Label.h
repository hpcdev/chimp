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
 * Declaration of filter::Label class. 
 */

#ifndef chimp_interaction_filter_Label_h
#define chimp_interaction_filter_Label_h

#include <chimp/interaction/filter/Base.h>

#include <string>
#include <sstream>
#include <set>

namespace chimp {
  namespace interaction {
    namespace filter {

      /** Filters an Equation based on a set of Input/Output terms. */
      struct Label : filter::Base {
        /* MEMBER FUNCTIONS */
        /** Default constructor does not initialize the xpath filter string. */
        Label( const std::string & label ) {
          xpath_query = "self::node()[@model='" + label + "']";
        }

      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_Label_h
