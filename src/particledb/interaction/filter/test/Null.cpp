/** \file
 * Test file for the Null class.
 * */
#define BOOST_TEST_MODULE  Null


#include <particledb/interaction/filter/Null.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Null_tests ); // {

  BOOST_AUTO_TEST_CASE( not_filter ) {
    using particledb::interaction::filter::Null;

    Null f;

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval("//Interaction");
    set set_in(xl.begin(), xl.end());

    set set_out = f.filter(set_in);

    BOOST_CHECK_EQUAL( set_in == set_out, true );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
