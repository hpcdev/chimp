/** \file
 * Test file for the EqIO class.
 * */
#define BOOST_TEST_MODULE  EqIO


#include <particledb/interaction/filter/EqIO.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( EqIO_tests ); // {

  BOOST_AUTO_TEST_CASE( query_generation ) {
    using particledb::interaction::filter::EqIO;
    EqIO f(EqIO::IN, "e^-", "Hg");

    BOOST_CHECK_EQUAL( get_xpath_query("In",f.terms),
      "Eq[count(In/P) = 2]/In"
      "/P[text()='Hg' and (number(@M)=1 or string(@M)='')]/.."
      "/P[text()='e^-' and (number(@M)=1 or string(@M)='')]/.."
      "/../.."
    );

    BOOST_CHECK_EQUAL( get_xpath_query("Out",f.terms),
      "Eq[count(Out/P) = 2]/Out"
      "/P[text()='Hg' and (number(@M)=1 or string(@M)='')]/.."
      "/P[text()='e^-' and (number(@M)=1 or string(@M)='')]/.."
      "/../.."
    );
  }

  BOOST_AUTO_TEST_CASE( input_filter ) {
    using particledb::interaction::filter::EqIO;
    EqIO f(EqIO::IN, "e^-", "Hg");

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "(e^-)+(Hg)->(e^-)+(Hg)\n"
      "(e^-)+(Hg)->(e^-)+(Hg(6-3P0))\n"
      "(e^-)+(Hg)->(e^-)+(Hg(6-3P1))\n"
      "(e^-)+(Hg)->(e^-)+(Hg(6-3P2))\n"
      "(e^-)+(Hg)->(e^-)+(Hg(7-3S1))\n"
      "(e^-)+(Hg)->(e^-)+(Hg^+)\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

  BOOST_AUTO_TEST_CASE( output_filter ) {
    using particledb::interaction::filter::EqIO;
    EqIO f(EqIO::OUT, "e^-", "Hg");

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "(e^-)+(Hg)->(e^-)+(Hg)\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
