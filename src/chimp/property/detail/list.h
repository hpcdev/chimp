/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
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

#ifndef chimp_property_detail_list_h
#define chimp_property_detail_list_h

#include <ostream>

namespace chimp {
  namespace property {
    namespace detail {

      /** The end-of-list tag. */
      struct Null {};

      /* ****** BEGIN List SPECIALIZATIONS ******* */
      /* Note that the second template argument in the list is supposed to be a
       * list. */

      template < typename P, typename L >
      struct List : P, L {
        /** Printer for aggregated properties. */
        std::ostream & print( std::ostream & out,
                              const std::string & sep = ", ") const {
          P::print(out,sep);
          L::print(out,sep);
          return out;
        }

        /** Load properties from the particular node in the (xml)
         * database/document. */
        template < typename DBnode >
        static List load( const DBnode & x ) {
          List l;
          l.P::operator=(P::load(x));
          l.L::operator=(L::load(x));
          return l;
        }
      };

      /** Covers the case of <P, Null>. */
      template < typename P >
      struct List< P, Null > : P {
        /** Printer for aggregated properties. */
        std::ostream & print( std::ostream & out,
                              const std::string & sep = ", ") const {
          P::print(out,sep);
          return out;
        }

        /** Load properties from the particular node in the (xml)
         * database/document. */
        template < typename DBnode >
        static List load( const DBnode & x ) {
          List l;
          l.P::operator=(P::load(x));
          return l;
        }
      };
      /* ****** END List SPECIALIZATIONS ******* */


      /** Insertion operation for particle properties List type. */
      template < typename P, typename L >
      inline std::ostream & operator<< ( std::ostream & out,
                                         const List<P,L> & agg ) {
        return agg.print(out, ", ");
      }





      /* ****** BEGIN Concat SPECIALIZATIONS ******* */
      template< typename P, typename L > struct Concat {
        typedef List< P, L > type;
      };

      template< typename P0, typename L0, typename P1, typename L1 >
      struct Concat< List< P0, L0 >, List< P1, L1 > > {
        typedef List< P0, typename Concat< L0, List< P1, L1 > >::type > type;
      };

      template< typename P0, typename P1, typename L0 >
      struct Concat< List<P0, Null>, List< P1, L0 > > {
        typedef List< P0, List< P1, L0 > > type;
      };

      template< typename P0, typename P1, typename L0 >
      struct Concat< P0, List< P1, L0 > > {
        typedef List< P0, List< P1, L0 > > type;
      };

      template< typename P0, typename L0, typename P1 >
      struct Concat< List< P0, L0 >, P1 > {
        typedef typename Concat< List< P0, L0 >, List< P1, Null > >::type type;
      };

      template< typename P0, typename L0 >
      struct Concat< List< P0, L0 >, Null > {
        typedef List< P0, L0 > type;
      };
      /* ****** END Concat SPECIALIZATIONS ******* */

    }/* namespace chimp::property::detail */
  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_detail_list_h
