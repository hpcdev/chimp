/** \file
 * Test file for the Elastic class.
 * */
#define BOOST_TEST_MODULE  Elastic


#include <particledb/interaction/filter/Elastic.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Elastic_tests ); // {

  BOOST_AUTO_TEST_CASE( filter ) {
    using particledb::interaction::filter::Elastic;
    using particledb::interaction::filter::is_elastic;
    Elastic f;

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval(
      "//Interaction"
      "/Eq[count(In/P) = 2]/In"
      "/P[text()='Hg' and (number(@M)=1 or string(@M)='')]/.."
      "/P[text()='e^-' and (number(@M)=1 or string(@M)='')]/.."
      "/../.."
    );
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "(e^-)+(Hg)->(e^-)+(Hg)\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      BOOST_CHECK_EQUAL( is_elastic(*i), true );

      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
