/** \file
 * Test file for the And class.
 * */
#define BOOST_TEST_MODULE  And


#include <particledb/interaction/filter/EqIO.h>
#include <particledb/interaction/filter/And.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( And_tests ); // {

  BOOST_AUTO_TEST_CASE( input_filter ) {
    using particledb::interaction::filter::EqIO;
    using particledb::interaction::filter::And;
    using boost::shared_ptr;
    And f( shared_ptr<EqIO>(new EqIO(EqIO::IN,  "e^-", "Hg")),
           shared_ptr<EqIO>(new EqIO(EqIO::OUT, "e^-", "Hg(6-3P0)")) );

    namespace xml = olson_tools::xml;
    xml::XMLDoc xmlDb(PARTICLEDB_XML);

    typedef xml::XMLContext::set set;
    xml::XMLContext::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    const char * filter_ans = 
      "(e^-)+(Hg)->(e^-)+(Hg(6-3P0))\n";
     
    std::ostringstream af;
    for ( set::iterator i = ans.begin(); i != ans.end(); ++i ) {
      af << i->query<std::string>("Eq") << '\n';
    }

    BOOST_CHECK_EQUAL( af.str(), filter_ans );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
