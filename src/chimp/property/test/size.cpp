/** \file
 * Test file for the property::size class.
 * */
#define BOOST_TEST_MODULE  size

#include <chimp/property/size.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( property_size ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    using chimp::property::size;
    {
      size s;
      BOOST_CHECK_EQUAL(s.value,1);
      BOOST_CHECK_EQUAL(s.size::value,1);
    }

    {
      size s(10);
      BOOST_CHECK_EQUAL(s.value,10);
      BOOST_CHECK_EQUAL(s.size::value,10);
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
