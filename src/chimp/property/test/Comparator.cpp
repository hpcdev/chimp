/** \file
 * Test file for the property::Comparator class.
 * */
#define BOOST_TEST_MODULE  Comparator

#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>
#include <chimp/property/Comparator.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using chimp::property::Add;
  using chimp::property::name;
  using chimp::property::mass;
  using chimp::property::Comparator;
  typedef Add < name, mass > Prop;
}

BOOST_AUTO_TEST_SUITE( property_Comparator ); // {

  BOOST_AUTO_TEST_CASE( comparisons ) {
    BOOST_CHECK_EQUAL(
      Comparator()( Prop("H", 1), Prop("H", 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("H", 1), Prop("He", 1) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 1), Prop("H", 10) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 1), Prop("H", 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop("He", 10), Prop("H", 1) ),
      false
    );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
