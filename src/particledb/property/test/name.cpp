/** \file
 * Test file for the property::name class.
 * */
#define BOOST_TEST_MODULE  name

#include <particledb/property/name.h>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( property_name ); // {

  BOOST_AUTO_TEST_CASE( intantiation ) {
    using particledb::property::name;
    {
      name n;
      BOOST_CHECK_EQUAL(n.value,"");
      BOOST_CHECK_EQUAL(n.name::value,"");
    }

    {
      name n(std::string("hi there"));
      BOOST_CHECK_EQUAL(n.value,"hi there");
      BOOST_CHECK_EQUAL(n.name::value,"hi there");
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
