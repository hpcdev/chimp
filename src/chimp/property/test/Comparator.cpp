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
 * Test file for the property::Comparator class.
 * */
#define BOOST_TEST_MODULE  Comparator

#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>
#include <chimp/property/Comparator.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using chimp::property::Add;
  using chimp::property::name;
  using chimp::property::mass;
  using chimp::property::Comparator;
  typedef Add < name, mass > Prop;
}

BOOST_AUTO_TEST_SUITE( property_Comparator ); // {

  BOOST_AUTO_TEST_CASE( comparisons ) {
    BOOST_CHECK_EQUAL(
      Comparator()( Prop("H", 1), Prop("H", 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("H", 1), Prop("He", 1) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 1), Prop("H", 10) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 1), Prop("H", 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 10), Prop("H", 1) ),
      false
    );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
