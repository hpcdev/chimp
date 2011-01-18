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



#ifndef chimp_property_detail_check_h
#define chimp_property_detail_check_h

#include <xylose/xml/Doc.h>
#include <xylose/xml/physical_parse.h>

#include <physical/runtime.h>

#include <string>

namespace chimp {
  namespace property {
    namespace detail {

      namespace xml = xylose::xml;

      /** Non-dimension used as default arg of Generic class.  This is used when
       * a physical dimension is not specified. */
      template <typename>
      struct NullDimension {};

      template < typename T, template<typename> class dimT >
      struct check {
        check(const T& t = T()) : value(t) {}
        T value;
      };

      /** Generic implementation of parse_item< check<?,?> >.
       * @tparam T
       *    The storage type of the item that we should be assigning (int,
       *    float, double, ...).
       *
       * @tparam dimT
       *    The physical units dimension that should be checked against the
       *    parsed quantity ( e.g. runtime::physical::dimension::energy ).
       */
      template < typename T, template<typename> class dimT >
      inline void parse_item( check<T,dimT> & out,
                              const xml::Context & x ) {
        using runtime::physical::Quantity;
        using runtime::physical::system::si;
        x.parse<Quantity>().assertMatch( dimT<si>::value() ).getCoeff(out.value);
      }

      template < typename T >
      inline void parse_item( check<T,NullDimension> & out,
                              const xml::Context & x ) {
        out.value = x.parse<T>();
      }

    }/* namespace chimp::property::detail */
  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_detail_check_h
