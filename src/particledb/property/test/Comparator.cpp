/** \file
 * Test file for the property::Comparator class.
 * */
#define BOOST_TEST_MODULE  Comparator

#include <particledb/property/Add.h>
#include <particledb/property/name.h>
#include <particledb/property/mass.h>
#include <particledb/property/Comparator.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

namespace {
  using particledb::property::Add;
  using particledb::property::name;
  using particledb::property::mass;
  using particledb::property::Comparator;
  typedef Add < name, mass > Prop;
}

BOOST_AUTO_TEST_SUITE( property_Comparator ); // {

  BOOST_AUTO_TEST_CASE( comparisons ) {
    BOOST_CHECK_EQUAL(
      Comparator()( Prop(std::string("H"), 1), Prop(std::string("H"), 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop(std::string("H"), 1), Prop(std::string("He"), 1) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop(std::string("He"), 1), Prop(std::string("H"), 10) ),
      true
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop(std::string("He"), 1), Prop(std::string("H"), 1) ),
      false
    );

    BOOST_CHECK_EQUAL(
      Comparator()( Prop(std::string("He"), 10), Prop(std::string("H"), 1) ),
      false
    );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
