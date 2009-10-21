/** \file
 * Test file for the Section class.
 * */
#define BOOST_TEST_MODULE  Section


#include <chimp/interaction/filter/Base.h>
#include <chimp/interaction/filter/EqIO.h>
#include <chimp/interaction/filter/Section.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Section_tests ); // {

  BOOST_AUTO_TEST_CASE( standard_filter ) {
    using chimp::interaction::filter::Section;

    Section f;

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_preferred ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard");

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("/ParticleDB/standard//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

  BOOST_AUTO_TEST_CASE( nonstandard_filter_required ) {
    using chimp::interaction::filter::Section;

    Section f("unstandard", Section::REQUIRED);

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

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

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

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
