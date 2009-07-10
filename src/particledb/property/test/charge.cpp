/** \file
 * Test file for the property::charge class.
 * */
#define BOOST_TEST_MODULE  charge

#include <particledb/property/charge.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( property_charge ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    using particledb::property::charge;
    {
      charge c;
      BOOST_CHECK_EQUAL(c.value,0);
      BOOST_CHECK_EQUAL(c.charge::value,0);
    }

    {
      charge c(10);
      BOOST_CHECK_EQUAL(c.value,10);
      BOOST_CHECK_EQUAL(c.charge::value,10);
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
