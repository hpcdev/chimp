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
 * Test file for the Section class.
 * */
#define BOOST_TEST_MODULE  Section


#include <chimp/interaction/filter/Base.h>
#include <chimp/interaction/filter/EqIO.h>
#include <chimp/interaction/filter/Section.h>

#include <xylose/XSTR.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Section_tests ); // {

  BOOST_AUTO_TEST_CASE( standard_filter ) {
    using chimp::interaction::filter::Section;

    Section f;

    namespace xml = xylose::xml;
    xml::Doc xmlDb(XSTR(CHIMP_PARTICLEDB_XML));

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_preferred ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard");

    namespace xml = xylose::xml;
    xml::Doc xmlDb(XSTR(CHIMP_PARTICLEDB_XML));

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_required ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard", Section::REQUIRED);

    namespace xml = xylose::xml;
    xml::Doc xmlDb(XSTR(CHIMP_PARTICLEDB_XML));

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_out.empty(), true );
  }

  BOOST_AUTO_TEST_CASE( standard_filter_required_EqIO_subfilter ) {
    using chimp::interaction::filter::Base;
    using chimp::interaction::filter::Section;
    using chimp::interaction::filter::EqIO;
    typedef boost::shared_ptr<Base> SP;

    Section f( "standard", Section::REQUIRED,
               SP(new EqIO( EqIO::OUT, "e^-", "Hg") )
             );

    namespace xml = xylose::xml;
    xml::Doc xmlDb(XSTR(CHIMP_PARTICLEDB_XML));

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    {
      const char * filter_ans = 
        "e^- + Hg  -->  e^- + Hg\n";
       
      std::ostringstream af;
      for ( set::iterator i = set_out.begin(); i != set_out.end(); ++i ) {
        af << i->query<std::string>("Eq") << '\n';
      }

      BOOST_CHECK_EQUAL( af.str(), filter_ans );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
