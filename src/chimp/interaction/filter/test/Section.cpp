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


#include <chimp/default_data.h>
#include <chimp/interaction/filter/Base.h>
#include <chimp/interaction/filter/EqIO.h>
#include <chimp/interaction/filter/Section.h>

#include <xylose/XSTR.h>
#include <xylose/logger.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

#ifndef OTHER_SECTION
#  error OTHER_SECTION filename is supposed to be specified by compile command
#endif


BOOST_AUTO_TEST_SUITE( Section_tests ); // {

  BOOST_AUTO_TEST_CASE( standard_filter ) {
    using chimp::interaction::filter::Section;

    Section f;

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( empty_section_issues_warning ) {
    xylose::logger::log_info("A warning should be issued for this test...");
    using chimp::interaction::filter::Section;

    Section f;
    f.section = "";

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_preferred_no_other_section ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard");

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_preferred_yes_other_section ) {
    using chimp::interaction::filter::Section;

    Section f("test");

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );
    {
      xml::Doc otherDoc( XSTR(OTHER_SECTION) );
      xmlDb.root_context.extend( otherDoc.root_context );
    }

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_out.size(), set_in.size() - 1u );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_required_empty_result ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard", Section::REQUIRED);

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_out.empty(), true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_required_dual_possibilities ) {
    using chimp::interaction::filter::Section;

    Section f("test", Section::REQUIRED);

    namespace xml = xylose::xml;
    xml::Doc xmlDb( chimp::default_data::particledb() );
    {
      xml::Doc otherDoc( XSTR(OTHER_SECTION) );
      xmlDb.root_context.extend( otherDoc.root_context );
    }

    typedef xml::Context::set  set;
    typedef xml::Context::list list;
    {
      list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
      set set_in(xl.begin(), xl.end());

      set set_out = f.filter(set_in);

      BOOST_CHECK_EQUAL( set_out.size(), 0u );
    }
    {
      list xl = xmlDb.eval("/ParticleDB//Interaction");
      set set_in(xl.begin(), xl.end());

      set set_out = f.filter(set_in);

      BOOST_CHECK_EQUAL( set_out.size(), 1u );
      BOOST_CHECK_EQUAL(
        set_out.begin()->query< std::string >("SomeNode", ""),
        "Some node"
      );
    }
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
    xml::Doc xmlDb( chimp::default_data::particledb() );

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
