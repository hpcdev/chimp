/** \file
 * Test file for the property::DefaultSet class.
 * */
#define BOOST_TEST_MODULE  DefaultSet

#include <chimp/property/DefaultSet.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using chimp::property::DefaultSet;
}

BOOST_AUTO_TEST_SUITE( property_DefaultSet ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    {
      DefaultSet p;
      BOOST_CHECK_EQUAL(p.name::value, "");
      BOOST_CHECK_EQUAL(p.size::value, 1);
      BOOST_CHECK_EQUAL(p.mass::value, 0);
      BOOST_CHECK_EQUAL(p.charge::value, 0);
    }

    {
      DefaultSet p( "bob", 10, 1, 2 );
      BOOST_CHECK_EQUAL(p.name::value, "bob");
      BOOST_CHECK_EQUAL(p.size::value, 10);
      BOOST_CHECK_EQUAL(p.mass::value, 1);
      BOOST_CHECK_EQUAL(p.charge::value, 2);
    }
  }

  BOOST_AUTO_TEST_CASE( printing ) {
    DefaultSet p("bob", 10, 1, 2);

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(p.print(ostr)).str(),
        "@name: bob, size: 10, mass: 1, charge: 2, "
      );
    }

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(ostr << p).str(),
        "@name: bob, size: 10, mass: 1, charge: 2, "
      );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
