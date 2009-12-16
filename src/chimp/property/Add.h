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
 * Definition of property Add template meta-function.
 */

#ifndef chimp_property_Add_h
#define chimp_property_Add_h

#include <chimp/property/Null.h>

#include <ostream>

namespace chimp {
  namespace property {

    /** Aggregates several properties into one class by inheritance.
     * The template parameters are particle properties to aggregated together so
     * that each is loaded from the xml data set at the appropriate time. <br>
     * Except, for P0, each of the parameters has a default value of
     * chimp::property::Null which is essentially a NO-OP class.
     */
    template < typename P0,
               typename P1  = Null<0>,
               typename P2  = Null<1>,
               typename P3  = Null<2>,
               typename P4  = Null<3>,
               typename P5  = Null<4>,
               typename P6  = Null<5>,
               typename P7  = Null<6>,
               typename P8  = Null<7>,
               typename P9  = Null<8>,
               typename P10 = Null<9>,
               typename P11 = Null<10> >
    struct Add : P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11 {

      /** Constructor with default arguments for each aggregated type. */
      Add( const P0  & p0  = P0(),
           const P1  & p1  = P1(),
           const P2  & p2  = P2(),
           const P3  & p3  = P3(),
           const P4  & p4  = P4(),
           const P5  & p5  = P5(),
           const P6  & p6  = P6(),
           const P7  & p7  = P7(),
           const P8  & p8  = P8(),
           const P9  & p9  = P9(),
           const P10 & p10 = P10(),
           const P11 & p11 = P11() )
        :
          P0 (p0),
          P1 (p1),
          P2 (p2),
          P3 (p3),
          P4 (p4),
          P5 (p5),
          P6 (p6),
          P7 (p7),
          P8 (p8),
          P9 (p9),
          P10(p10),
          P11(p11)
      { }

      /** Printer for aggregated properties. */
      std::ostream & print(std::ostream & out) const {
        P0::super::print(out,", ");
        P1::super::print(out,", ");
        P2::super::print(out,", ");
        P3::super::print(out,", ");
        P4::super::print(out,", ");
        P5::super::print(out,", ");
        P6::super::print(out,", ");
        P7::super::print(out,", ");
        P8::super::print(out,", ");
        P9::super::print(out,", ");
        P10::super::print(out,", ");
        P11::super::print(out,", ");
        return out;
      }

      /** Load properties from the particular node in the (xml)
       * database/document. */
      template <class DBnode>
      static Add load(const DBnode & x) {
        Add t;
        t.P0::super::operator=(P0::load(x));
        t.P1::super::operator=(P1::load(x));
        t.P2::super::operator=(P2::load(x));
        t.P3::super::operator=(P3::load(x));
        t.P4::super::operator=(P4::load(x));
        t.P5::super::operator=(P5::load(x));
        t.P6::super::operator=(P6::load(x));
        t.P7::super::operator=(P7::load(x));
        t.P8::super::operator=(P8::load(x));
        t.P9::super::operator=(P9::load(x));
        t.P10::super::operator=(P10::load(x));
        t.P11::super::operator=(P11::load(x));
        return t;
      }
    };

    /** Insertion operation for the aggregate particle properties type. */
    template < typename P0, typename P1, typename P2, typename P3,
               typename P4, typename P5, typename P6, typename P7,
               typename P8, typename P9, typename P10,typename P11 >
    inline std::ostream & operator<< (
      std::ostream & out,
      const Add<P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11> & agg
    ) {
      return agg.print(out);
    }

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_Add_h
