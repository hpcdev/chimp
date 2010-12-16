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
 * Test file for the Elastic class.
 * */
#define BOOST_TEST_MODULE  Elastic


#include <chimp/interaction/filter/Elastic.h>

#include <xylose/XSTR.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Elastic_tests ); // {

  BOOST_AUTO_TEST_CASE( filter ) {
    using chimp::interaction::filter::Elastic;
    using chimp::interaction::filter::isElastic;
    Elastic f;

    namespace xml = xylose::xml;
    xml::Doc xmlDb(XSTR(CHIMP_PARTICLEDB_XML));

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval(
      "//Interaction"
      "/Eq[count(In/T) = 2]/In"
      "/T[string(P)='Hg' and (number(n)=1 or string(n)='')]/.."
      "/T[string(P)='e^-' and (number(n)=1 or string(n)='')]/.."
      "/../.."
    );
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "e^- + Hg  -->  e^- + Hg\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      BOOST_CHECK_EQUAL( isElastic(*i), true );

      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
