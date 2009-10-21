/** \file
 * Test file for the Label class.
 * */
#define BOOST_TEST_MODULE  Label


#include <chimp/interaction/filter/Label.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Label_tests ); // {

  BOOST_AUTO_TEST_CASE( input_filter ) {
    using chimp::interaction::filter::Label;
    using boost::shared_ptr;
    Label f( "elastic" );

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      BOOST_CHECK_EQUAL( i->query<std::string>("@type"), "elastic" );
    }

  }

BOOST_AUTO_TEST_SUITE_END(); // }
