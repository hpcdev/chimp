/** \file
 * Test file for the Elastic class.
 * */
#define BOOST_TEST_MODULE  Elastic


#include <particledb/RuntimeDB.h>
#include <particledb/interaction/Input.h>
#include <particledb/interaction/Particle.h>
#include <particledb/interaction/model/Elastic.h>
#include <particledb/interaction/model/test/diagnostics.h>

#include <olson-tools/random/random.h>

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <limits>
#include <cmath>

namespace {
  using boost::shared_ptr;
  using particledb::interaction::Particle;
  using namespace particledb::interaction::model::test;
  namespace xml = olson_tools::xml;
}

BOOST_AUTO_TEST_SUITE( Elastic_tests ); // {

  BOOST_AUTO_TEST_CASE( conservation ) {
    typedef particledb::RuntimeDB<> DB;
    DB db;
    db.addParticleType("87Rb");
    int part_i = db.findParticleIndx("87Rb");
    uint32 seed = 1;
    MTRNGseedV1(&seed);

    typedef particledb::interaction::model::Elastic<DB::options> Elastic;
    particledb::interaction::Input input(part_i,part_i);
    shared_ptr<Elastic> el( Elastic().new_load(xml::XMLContext(), input, db) );

    BOOST_CHECK_EQUAL( el->getLabel(), "elastic" );

    {
      /* this first test, we'll just test the output of one statically */
      Particle p0i, p1i,
               p0f( V3(-8.2978,49.7185,22.0324),   V3(-15.1638,-67.6912,-4.80631) ),
               p1f( V3(-19.7667,49.9041,-35.3244), V3(32.0284,-23.0635,-42.7232) );
      randomize(p0i);
      randomize(p1i);

      el->interact(p0i,p1i);

      BOOST_CHECK_EQUAL( (p0i.v-p0f.v).abs() <= 2.3186794142094168e-05, true);
      BOOST_CHECK_EQUAL( (p1i.v-p1f.v).abs() <= 4.9346339874592464e-05, true);
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

        double energyi = energy(p0, part_i, db) +
                         energy(p1, part_i, db);
        Vector<double,3> momentumi =
                         momentum(p0, part_i, db) +
                         momentum(p1, part_i, db);
        double vrel = (p0.v - p1.v).abs();
        Vector<double,3> vcm  = 0.5*(p0.v + p1.v);
        vcm.save_fabs();

        Vector<double,3> p_eps = 6*eps *
          V3( std::max(vrel/vcm[0], vcm[0]/vrel),
              std::max(vrel/vcm[1], vcm[1]/vrel),
              std::max(vrel/vcm[2], vcm[2]/vrel) );

        double e_eps = 6*eps;
        
        el->interact(p0,p1);

        double energyf = energy(p0, part_i, db) +
                         energy(p1, part_i, db);
        Vector<double,3> momentumf =
                         momentum(p0, part_i, db) +
                         momentum(p1, part_i, db);
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
          (dP2 - compMult(dP,dP)).save_sqrt() / std::sqrt(N);

        dP.save_fabs();
        BOOST_CHECK_EQUAL( dP <= (2.*dP_mean_sigma), true );
      }
    }
  }

BOOST_AUTO_TEST_SUITE_END(); // }
