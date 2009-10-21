/** \file
 * Test file for the  RuntimeDB class.
 * */
#define BOOST_TEST_MODULE  RuntimeDB


#include <chimp/RuntimeDB.h>
#include <chimp/interaction/filter/Or.h>
#include <chimp/interaction/filter/Elastic.h>
#include <chimp/interaction/filter/Label.h>

#include <boost/test/unit_test.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE( RuntimeDB_tests ); // {

  BOOST_AUTO_TEST_CASE( default_filter_initBinaryInteraction_simple ) {
    typedef chimp::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    db.initBinaryInteractions();

    DB::Set set = db(0,0);

    BOOST_CHECK_EQUAL( set.rhs.size(), 1u );

    typedef DB::Set::Equation::list::iterator EIter;

    {
      std::ostringstream estr;
      for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
        i->print( estr, db ) << '\n';
        BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
      }

      BOOST_CHECK_EQUAL( estr.str(), "2 87Rb  -->  2 87Rb\n" );
    }
  }

  BOOST_AUTO_TEST_CASE( default_filter_initBinaryInteraction_2part ) {
    typedef chimp::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    db.addParticleType("85Rb");
    db.initBinaryInteractions();

    int i87Rb = db.findParticleIndx("87Rb");
    int i85Rb = db.findParticleIndx("85Rb");

    {/* 87Rb * 87Rb */
      DB::Set set = db(i87Rb,i87Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 1u );

      typedef DB::Set::Equation::list::iterator EIter;

      {
        std::ostringstream estr;
        for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
          i->print( estr, db ) << '\n';
          BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
        }

        BOOST_CHECK_EQUAL( estr.str(), "2 87Rb  -->  2 87Rb\n" );
      }
    }/* 87Rb * 87Rb */

    {/* 85Rb * 85Rb */
      DB::Set set = db(i85Rb,i85Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 1u );

      typedef DB::Set::Equation::list::iterator EIter;

      {
        std::ostringstream estr;
        for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
          i->print( estr, db ) << '\n';
          BOOST_CHECK_EQUAL( i->interaction->getLabel(), "elastic" );
        }

        BOOST_CHECK_EQUAL( estr.str(), "2 85Rb  -->  2 85Rb\n" );
      }
    }/* 85Rb * 85Rb */

    {/* 87Rb * 85Rb */
      DB::Set set = db(i85Rb,i87Rb);

      BOOST_CHECK_EQUAL( set.rhs.size(), 0u );
    }/* 85Rb * 85Rb */
  }

  BOOST_AUTO_TEST_CASE( set_filter_iterate_interactions ) {
    namespace filter = chimp::interaction::filter;
    typedef boost::shared_ptr<filter::Base> SP;

    typedef chimp::RuntimeDB<> DB;
    DB db;
    db.addParticleType("e^-");
    db.addParticleType("Hg");
    db.addParticleType("Hg^+");

    db.filter =
      SP(
        new filter::Or( SP(new filter::Elastic),
                        SP(new filter::Label("inelastic")) )
      );


    db.initBinaryInteractions();

    // the size of the table should be 6
    BOOST_CHECK_EQUAL( db.getInteractions().size(), 6u );

    typedef std::vector<DB::Set>::const_iterator CIter;
    CIter end = db.getInteractions().end();
    unsigned int number_interactions = 0u;
    for ( CIter i = db.getInteractions().begin(); i != end; ++i ) {
      number_interactions += i->rhs.size();
    }

    // NOTE:  if we add e- + e-, or Hg+Hg, or Hg+ + Hg+ type of collisions data,
    // we will have to change this number
    BOOST_CHECK_EQUAL( number_interactions, 3u );

  }

BOOST_AUTO_TEST_SUITE_END(); // }
