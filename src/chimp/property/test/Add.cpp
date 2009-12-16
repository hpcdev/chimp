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
 * Test file for the property::Add class.
 * */
#define BOOST_TEST_MODULE  Add

#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using chimp::property::Add;
  using chimp::property::name;
  using chimp::property::mass;
  typedef Add < name, mass > Prop;
}

BOOST_AUTO_TEST_SUITE( property_Add ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    {
      Prop p;
      BOOST_CHECK_EQUAL(p.name::value,"");
      BOOST_CHECK_EQUAL(p.mass::value,0);
    }

    {
      Prop p( "bob", 10 );
      BOOST_CHECK_EQUAL(p.name::value,"bob");
      BOOST_CHECK_EQUAL(p.mass::value,10);
    }
  }

  BOOST_AUTO_TEST_CASE( printing ) {
    Prop p("bob", 10.0);

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(p.print(ostr)).str(),
        "@name: bob, mass: 10, "
      );
    }

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(ostr << p).str(),
        "@name: bob, mass: 10, "
      );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
