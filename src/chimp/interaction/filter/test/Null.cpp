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
 * Test file for the Null class.
 * */
#define BOOST_TEST_MODULE  Null


#include <chimp/interaction/filter/Null.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Null_tests ); // {

  BOOST_AUTO_TEST_CASE( not_filter ) {
    using chimp::interaction::filter::Null;

    Null f;

    namespace xml = xylose::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
