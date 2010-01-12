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


#include <chimp/interaction/cross_section/detail/LotzDetails.h>

#include <olson-tools/xml/physical_parse.h>

#include <physical/runtime.h>
#include <physical/physical.h>

namespace chimp {
  namespace interaction {
    namespace cross_section {
      namespace detail {

        void parse_item( LotzParameters & out, const xml::Context & x ) {
          {
            using runtime::physical::Quantity;
            using runtime::physical::constant::si::eV;
            using runtime::physical::unit::m;
            /* Note that we use Quantity::assertUnitless() instead of parsing
             * directly to double so that we can have calculator support. */
            x.query<Quantity>("P").assertMatch(eV).getCoeff(out.P);
            x.query<Quantity>("q").assertUnitless().getCoeff(out.q);
            x.query<Quantity>("a").assertMatch(m*m*eV*eV).getCoeff(out.a);
            x.query<Quantity>("b").assertUnitless().getCoeff(out.b);
            x.query<Quantity>("c").assertUnitless().getCoeff(out.c);
          }

          /* now cache the result of beta */
          {
            using physical::constant::si::m_e;
            out.beta = m_e / ( 2. * out.P );
          }
        }

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */
