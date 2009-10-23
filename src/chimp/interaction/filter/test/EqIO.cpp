/** \file
 * Test file for the EqIO class.
 * */
#define BOOST_TEST_MODULE  EqIO


#include <chimp/interaction/filter/EqIO.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( EqIO_tests ); // {

  BOOST_AUTO_TEST_CASE( query_generation ) {
    using chimp::interaction::filter::EqIO;
    EqIO f(EqIO::IN, "e^-", "Hg");

    BOOST_CHECK_EQUAL( getXpathQuery("In",f.terms),
      "Eq[count(In/T) = 2]/In"
      "/T[string(P)='Hg' and (number(n)=1 or string(n)='')]/.."
      "/T[string(P)='e^-' and (number(n)=1 or string(n)='')]/.."
      "/../.."
    );

    BOOST_CHECK_EQUAL( getXpathQuery("Out",f.terms),
      "Eq[count(Out/T) = 2]/Out"
      "/T[string(P)='Hg' and (number(n)=1 or string(n)='')]/.."
      "/T[string(P)='e^-' and (number(n)=1 or string(n)='')]/.."
      "/../.."
    );
  }

  BOOST_AUTO_TEST_CASE( input_filter ) {
    using chimp::interaction::filter::EqIO;
    EqIO f(EqIO::IN, "e^-", "Hg");

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "e^- + Hg  -->  e^- + Hg\n"
      "e^- + Hg  -->  e^- + Hg(6-3P0)\n"
      "e^- + Hg  -->  e^- + Hg(6-3P1)\n"
      "e^- + Hg  -->  e^- + Hg(6-3P2)\n"
      "e^- + Hg  -->  e^- + Hg(7-3S1)\n"
      "e^- + Hg  -->  2 e^- + Hg^+\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

  BOOST_AUTO_TEST_CASE( output_filter ) {
    using chimp::interaction::filter::EqIO;
    EqIO f(EqIO::OUT, "e^-", "Hg");

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "e^- + Hg  -->  e^- + Hg\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
