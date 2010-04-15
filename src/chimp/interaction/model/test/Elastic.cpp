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
 * Test file for the Elastic class.
 * */
#define BOOST_TEST_MODULE  Elastic


#include <chimp/RuntimeDB.h>
#include <chimp/interaction/Term.h>
#include <chimp/interaction/Input.h>
#include <chimp/interaction/Particle.h>
#include <chimp/interaction/global_rng.h>
#include <chimp/interaction/ReducedMass.h>
#include <chimp/interaction/model/Elastic.h>
#include <chimp/interaction/model/test/diagnostics.h>

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <limits>
#include <cmath>

namespace {
  using boost::shared_ptr;
  using chimp::interaction::Particle;
  using chimp::interaction::Term;
  namespace test = chimp::interaction::model::test;
  using xylose::V3;
  using xylose::Vector;
  namespace xml = xylose::xml;
}

BOOST_AUTO_TEST_SUITE( Elastic_tests ); // {

  BOOST_AUTO_TEST_CASE( conservation ) {
    typedef chimp::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    int part_i = db.findParticleIndx("87Rb");

    typedef chimp::interaction::model::Elastic<DB::options> Elastic;
    chimp::interaction::global_rng.seed(1u);
    Term t0(part_i);
    chimp::interaction::Equation<DB::options> eq;
    eq.A = eq.B = t0;
    /* cache the reduced mass */
    eq.reducedMass = chimp::interaction::ReducedMass( eq, db );
    shared_ptr<Elastic> el( Elastic().new_load(xml::Context(), eq, db) );

    BOOST_CHECK_EQUAL( el->getLabel(), "elastic" );

    {
      /* this first test, we'll just test the output of one statically */
      Particle p0i, p1i,
               p0f( V3(-8.2978,49.7185,22.0324),   V3(-15.1638,-67.6912,-4.80631) ),
               p1f( V3(-19.7667,49.9041,-35.3244), V3(32.0284,-23.0635,-42.7232) );
      randomize(p0i);
      randomize(p1i);

      el->interact(p0i,p1i);

      BOOST_CHECK_LE( (p0i.v-p0f.v).abs(), 2.3186794143e-05);
      BOOST_CHECK_LE( (p1i.v-p1f.v).abs(), 4.9346339875e-05);
    }

    {
      /* we'll now test a whole bunch and check them for conservation laws */
      const double eps = std::numeric_limits<double>::epsilon();
      Vector<double,3> dP(0.0), dP2(0.0);
      const int N = 100000;

      for (int i = 0; i < N; ++i ) {
        Particle p0, p1;
        randomize(p0);
        randomize(p1);

        double energyi = test::energy(p0, part_i, db) +
                         test::energy(p1, part_i, db);
        Vector<double,3> momentumi =
                         test::momentum(p0, part_i, db) +
                         test::momentum(p1, part_i, db);
        double vrel = (p0.v - p1.v).abs();
        Vector<double,3> vcm  = 0.5*(p0.v + p1.v);
        vcm.save_fabs();

        Vector<double,3> p_eps = 6*eps *
          V3( std::max(vrel/vcm[0], vcm[0]/vrel),
              std::max(vrel/vcm[1], vcm[1]/vrel),
              std::max(vrel/vcm[2], vcm[2]/vrel) );

        double e_eps = 6*eps;
        
        el->interact(p0,p1);

        double energyf = test::energy(p0, part_i, db) +
                         test::energy(p1, part_i, db);
        Vector<double,3> momentumf =
                         test::momentum(p0, part_i, db) +
                         test::momentum(p1, part_i, db);
        using std::abs;
        using std::max;

        BOOST_CHECK_EQUAL( max(abs(1-energyf/energyi), e_eps), e_eps);

        BOOST_CHECK_EQUAL( max(abs(1-momentumi[0]/momentumf[0]), p_eps[0]), p_eps[0]);
        BOOST_CHECK_EQUAL( max(abs(1-momentumi[1]/momentumf[1]), p_eps[1]), p_eps[1]);
        BOOST_CHECK_EQUAL( max(abs(1-momentumi[2]/momentumf[2]), p_eps[2]), p_eps[2]);

        {
          Vector<double,3> d = (momentumf - momentumi) / db[part_i].mass::value;
          dP += d;
          dP2 += compMult(d,d);
        }

      }

      {
        dP /= N;
        dP2 /= N;

        Vector <double,3> dP_mean_sigma =
          (dP2 - compMult(dP,dP)).save_sqrt() /
          std::sqrt(static_cast<double>(N));

        dP.save_fabs();
        BOOST_CHECK_LE( dP, (2.*dP_mean_sigma) );
      }
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
