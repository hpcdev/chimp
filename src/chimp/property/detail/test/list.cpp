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
 * Unit test for chimp/property/detail/list.h
 */

#define BOOST_TEST_MODULE  chimp_property_detail_list

#include <chimp/property/detail/list.h>
#include <chimp/property/aggregate.h>

#include <boost/test/unit_test.hpp>

namespace {
  using chimp::property::Aggregate;
  using chimp::property::detail::Concat;
  using chimp::property::detail::List;
  using chimp::property::detail::Null;

  struct A {
    double value;
  };
  
  struct B {
    int value;
  };
  
  struct C {
    char value;
  };
  
  struct D {
    float value;
  };
  
  struct E {
    int value;
  };
}

BOOST_AUTO_TEST_CASE( intantiation ) {
  /* If this compiles, we should be pretty safe. */
  typedef List< A, List< B, Null > >                      List0;
  typedef List< C, Null >                                 List1;
  typedef Concat< List<A, List< B, Null > >, Null >::type List2;
  typedef Concat< List0, List1 >::type                    List3;
  typedef Concat< List1, List0 >::type                    List4;
  typedef Concat< D, List4 >::type                        List5;
  typedef Concat< List5, E >::type                        List6;
  typedef Aggregate< A, B, C >::type                      List7;
  typedef Aggregate< A, B, C, D, E >::type                List8;
  typedef Aggregate< List1, List0 >::type                 List9;

  List0 l0 = List< A, List< B, Null > >();
  List1 l1 = List< C, Null >();
  List2 l2 = List< A, List< B, Null > >();
  List3 l3 = List< A, List< B, List< C, Null > > >();
  List4 l4 = List< C, List< A, List< B, Null > > >();
  List5 l5 = List< D, List< C, List< A, List< B, Null > > > >();
  List6 l6 = List< D, List< C, List< A, List< B, List< E, Null > > > > >();
  List7 l7 = List< A, List< B, List< C, Null > > >();
  List8 l8 = List< A, List< B, List< C, List< D, List< E, Null > > > > >();
  List9 l9 = List< C, List< A, List< B, Null > > >();
}
