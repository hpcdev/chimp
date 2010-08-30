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
 * Test file for the  Equation class.
 * */
#define BOOST_TEST_MODULE  Equation


#include <chimp/RuntimeDB.h>
#include <chimp/make_options.h>
#include <chimp/interaction/Equation.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Equation_tests ); // {

  BOOST_AUTO_TEST_CASE( loading ) {
    typedef chimp::make_options<>::type options;
    chimp::RuntimeDB<options> db;
    db.addParticleType("87Rb");

    typedef chimp::interaction::Equation<options> Equation;
    Equation eq = Equation::load(db.xmlDb.root_context, "2 87Rb  -->  2 87Rb", db);

    BOOST_CHECK_EQUAL( eq.interaction->getLabel(), "elastic" );
    BOOST_CHECK_EQUAL( eq.isElastic(), true );

    std::ostringstream sstr;
    BOOST_CHECK_EQUAL(
      static_cast<std::ostringstream&>(eq.print(sstr, db)).str(),
      "2 87Rb  -->  2 87Rb"
    );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
