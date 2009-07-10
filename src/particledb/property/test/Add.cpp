/** \file
 * Test file for the property::Add class.
 * */
#define BOOST_TEST_MODULE  Add

#include <particledb/property/Add.h>
#include <particledb/property/name.h>
#include <particledb/property/mass.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using particledb::property::Add;
  using particledb::property::name;
  using particledb::property::mass;
  typedef Add < name, mass > Prop;
}

BOOST_AUTO_TEST_SUITE( property_Add ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    {
      Prop p;
      BOOST_CHECK_EQUAL(p.name::value,"");
      BOOST_CHECK_EQUAL(p.mass::value,0);
    }

    {
      Prop p( std::string("bob"), 10 );
      BOOST_CHECK_EQUAL(p.name::value,"bob");
      BOOST_CHECK_EQUAL(p.mass::value,10);
    }
  }

  BOOST_AUTO_TEST_CASE( printing ) {
    Prop p(std::string("bob"), 10.0);

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(p.print(ostr)).str(),
        "@name: bob, mass: 10, "
      );
    }

    {
      std::ostringstream ostr;
      BOOST_CHECK_EQUAL(
        dynamic_cast<std::ostringstream&>(ostr << p).str(),
        "@name: bob, mass: 10, "
      );
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
