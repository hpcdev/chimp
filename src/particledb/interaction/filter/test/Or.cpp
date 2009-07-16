/** \file
 * Test file for the Or class.
 * */
#define BOOST_TEST_MODULE  Or


#include <particledb/interaction/filter/EqIO.h>
#include <particledb/interaction/filter/Or.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Or_tests ); // {

  BOOST_AUTO_TEST_CASE( input_filter ) {
    using particledb::interaction::filter::EqIO;
    using particledb::interaction::filter::Or;
    using boost::shared_ptr;
    Or f( shared_ptr<EqIO>(new EqIO(EqIO::IN,  "e^-", "Hg")),
          shared_ptr<EqIO>(new EqIO(EqIO::OUT, "e^-", "Kr")) );

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "e^- + Kr  -->  e^- + Kr\n"
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

BOOST_AUTO_TEST_SUITE_END(); // }
