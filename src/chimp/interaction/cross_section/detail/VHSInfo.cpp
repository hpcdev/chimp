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


#include <chimp/interaction/cross_section/detail/VHSInfo.h>

#include <xylose/xml/physical_parse.h>

namespace chimp {
  namespace interaction {
    namespace cross_section {
      namespace detail {

        void parse_item( VHSInfo & out, const xml::Context & x ) {
          using runtime::physical::Quantity;
          using runtime::physical::unit::K;
          using runtime::physical::unit::m;
          x.query<Quantity>("value").assertMatch(m*m).getCoeff(out.cross_section);
          x.query<Quantity>("T_ref").assertMatch(K).getCoeff(out.T_ref);
          x.query<Quantity>("visc_T_law").assertUnitless().getCoeff(out.visc_T_law);
          out.compute_gamma_visc_inv();
        }

        VHSInfo VHSInfo::load(const xml::Context & x) {
          return x.parse<VHSInfo>();
        }

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */
