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
 * Implementation of VSS helper functions.
 */

#include <chimp/interaction/model/detail/vss_helpers.h>

#include <xylose/xml/physical_parse.h>

#include <physical/runtime.h>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace detail {

        /** load a new instance of the Interaction. */
        double loadVSSParamInv( const xml::Context & x ) {
          using runtime::physical::Quantity;
          return x.query<Quantity>("vss_param_inv")
                  .assertUnitless().getCoeff<double>();
        }

      } /* namespace chimp::interaction::model::detail */
    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

