/** \file
 * Test file for the  Equation class.
 * */
#define BOOST_TEST_MODULE  Equation


#include <chimp/RuntimeDB.h>
#include <chimp/make_options.h>
#include <chimp/interaction/Equation.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( Equation_tests ); // {

  BOOST_AUTO_TEST_CASE( loading ) {
    typedef chimp::make_options<>::type options;
    chimp::RuntimeDB<options> db;
    db.addParticleType("87Rb");

    typedef chimp::interaction::Equation<options> Equation;
    Equation eq = Equation::load(db.xmlDb.root_context, "2 87Rb  -->  2 87Rb", db);

    BOOST_CHECK_EQUAL( eq.interaction->getLabel(), "elastic" );

    std::ostringstream sstr;
    BOOST_CHECK_EQUAL(
      static_cast<std::ostringstream&>(eq.print(sstr, db)).str(),
      "2 87Rb  -->  2 87Rb"
    );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
