/** \file
 * Test file for the property::mass class.
 * */
#define BOOST_TEST_MODULE  mass

#include <particledb/property/mass.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( property_mass ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    using particledb::property::mass;
    {
      mass m;
      BOOST_CHECK_EQUAL(m.value,0);
      BOOST_CHECK_EQUAL(m.mass::value,0);
    }

    {
      mass m(10);
      BOOST_CHECK_EQUAL(m.value,10);
      BOOST_CHECK_EQUAL(m.mass::value,10);
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
