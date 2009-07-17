/** \file
 * Test file for the Not class.
 * */
#define BOOST_TEST_MODULE  Not


#include <particledb/interaction/filter/EqIO.h>
#include <particledb/interaction/filter/Or.h>
#include <particledb/interaction/filter/Not.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Not_tests ); // {

  BOOST_AUTO_TEST_CASE( not_filter ) {
    using particledb::interaction::filter::Base;
    using particledb::interaction::filter::EqIO;
    using particledb::interaction::filter::Or;
    using particledb::interaction::filter::Not;
    typedef boost::shared_ptr<Base> SP;

    Not f(
      /* positive filter. */
      SP(new Or( SP(new EqIO(EqIO::IN,  "e^-", "Hg")),
                 SP(new EqIO(EqIO::OUT, "e^-", "Kr")) ) ),
      /* negative filter. */
      SP(new EqIO(EqIO::IN, "e^-", "Kr"))
         );

    namespace xml = olson_tools::xml;
    xml::Doc xmlDb(PARTICLEDB_XML);

    typedef xml::Context::set set;
    xml::Context::list xl = xmlDb.eval("//Interaction");
    set xset(xl.begin(), xl.end());

    set ans = f.filter(xset);

    {
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
  }

BOOST_AUTO_TEST_SUITE_END(); // }
