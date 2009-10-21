/** \file
 * Test file for the Elastic class.
 * */
#define BOOST_TEST_MODULE  Elastic


#include <chimp/interaction/filter/Elastic.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Elastic_tests ); // {

  BOOST_AUTO_TEST_CASE( filter ) {
    using chimp::interaction::filter::Elastic;
    using chimp::interaction::filter::is_elastic;
    Elastic f;

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

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
      BOOST_CHECK_EQUAL( is_elastic(*i), true );

      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
