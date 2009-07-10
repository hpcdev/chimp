/** \file
 * Test file for the  Equation class.
 * */
#define BOOST_TEST_MODULE  Equation


#include <particledb/RuntimeDB.h>
#include <particledb/make_options.h>
#include <particledb/interaction/Equation.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Equation_tests ); // {

  BOOST_AUTO_TEST_CASE( loading ) {
    typedef particledb::make_options<>::type options;
    particledb::RuntimeDB<options> db;
    db.addParticleType("87Rb");

    typedef particledb::interaction::Equation<options> Equation;
    Equation eq = Equation::load(db.xmlDb.root_context, "(87Rb)->(87Rb)", db);

    std::ostringstream sstr;
    BOOST_CHECK_EQUAL( static_cast<std::ostringstream&>(eq.print(sstr, db)).str(), "(87Rb)->(87Rb)");
  }

BOOST_AUTO_TEST_SUITE_END(); // }
