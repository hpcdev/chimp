/** \file
 * Test file for the  RuntimeDB class.
 * */
#define BOOST_TEST_MODULE  RuntimeDB


#include <particledb/RuntimeDB.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( RuntimeDB_tests ); // {

  BOOST_AUTO_TEST_CASE( default_filter_initBinaryInteraction_simple ) {
    typedef particledb::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    db.initBinaryInteractions();

    DB::Set set = db(0,0);

    BOOST_CHECK_EQUAL( set.rhs.size(), 1 );

    typedef DB::Set::Equation::list::iterator EIter;

    {
      std::ostringstream estr;
      for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
        i->print( estr, db ) << '\n';
        BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
      }

      BOOST_CHECK_EQUAL( estr.str(), "(87Rb)->(87Rb)\n" );
    }
  }

  BOOST_AUTO_TEST_CASE( default_filter_initBinaryInteraction_2part ) {
    typedef particledb::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    db.addParticleType("85Rb");
    db.initBinaryInteractions();

    int i87Rb = db.findParticleIndx("87Rb");
    int i85Rb = db.findParticleIndx("85Rb");

    {/* 87Rb * 87Rb */
      DB::Set set = db(i87Rb,i87Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 1 );

      typedef DB::Set::Equation::list::iterator EIter;

      {
        std::ostringstream estr;
        for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
          i->print( estr, db ) << '\n';
          BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
        }

        BOOST_CHECK_EQUAL( estr.str(), "(87Rb)->(87Rb)\n" );
      }
    }/* 87Rb * 87Rb */

    {/* 85Rb * 85Rb */
      DB::Set set = db(i85Rb,i85Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 1 );

      typedef DB::Set::Equation::list::iterator EIter;

      {
        std::ostringstream estr;
        for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
          i->print( estr, db ) << '\n';
          BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
        }

        BOOST_CHECK_EQUAL( estr.str(), "(85Rb)->(85Rb)\n" );
      }
    }/* 85Rb * 85Rb */

    {/* 87Rb * 85Rb */
      DB::Set set = db(i85Rb,i87Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 0 );
    }/* 85Rb * 85Rb */
  }


BOOST_AUTO_TEST_SUITE_END(); // }
