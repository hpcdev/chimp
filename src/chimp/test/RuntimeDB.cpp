/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Copyright (C) 2006, 2008 Spencer E. Olson                                   *
 *                                                                             *
 * This file is part of CHIMP                                                  *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


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

    typedef DB::InteractionTable::const_iterator CIter;
    CIter end = db.getInteractions().end();
    unsigned int number_interactions = 0u;
    for ( CIter i = db.getInteractions().begin(); i != end; ++i ) {
      number_interactions += i->rhs.size();
    }

    // NOTE:  if we add new collisions data, we will have to change these:
    BOOST_CHECK_EQUAL( number_interactions, 2u );

    BOOST_CHECK_EQUAL( db("e^-", "e^-" ).rhs.size(), 1u );
    BOOST_CHECK_EQUAL( db("e^-", "Hg"  ).rhs.size(), 1u );
    BOOST_CHECK_EQUAL( db("e^-", "Hg^+").rhs.size(), 0u );

    BOOST_CHECK_EQUAL( db("Hg",  "Hg"  ).rhs.size(), 0u );
    BOOST_CHECK_EQUAL( db("Hg",  "Hg^+").rhs.size(), 0u );

    BOOST_CHECK_EQUAL( db("Hg^+","Hg^+").rhs.size(), 0u );
  }

BOOST_AUTO_TEST_SUITE_END(); // }
