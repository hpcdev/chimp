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
 * Test file for the property::DefaultSet class.
 * */
#define BOOST_TEST_MODULE  DefaultSet

#include <chimp/property/DefaultSet.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using chimp::property::DefaultSet;
  typedef chimp::property::name N;
  typedef chimp::property::mass M;
  typedef chimp::property::charge C;
}

BOOST_AUTO_TEST_SUITE( property_DefaultSet ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    {
      DefaultSet p;
      BOOST_CHECK_EQUAL(p.name::value, "");
      BOOST_CHECK_EQUAL(p.mass::value, 0);
      BOOST_CHECK_EQUAL(p.charge::value, 0);
    }

    {
      DefaultSet p = make_properties( N("bob"), M(1), C(2) );
      BOOST_CHECK_EQUAL(p.name::value, "bob");
      BOOST_CHECK_EQUAL(p.mass::value, 1);
      BOOST_CHECK_EQUAL(p.charge::value, 2);
    }
  }

  BOOST_AUTO_TEST_CASE( printing ) {
    DefaultSet p = make_properties( N("bob"), M(1), C(2));

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(p.print(ostr)).str(),
        "@name: bob, mass: 1, charge: 2, "
      );
    }

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(ostr << p).str(),
        "@name: bob, mass: 1, charge: 2, "
      );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
