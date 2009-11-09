
#include "Particle.h"
#include "Cell.h"

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/filter/Or.h>
#include <chimp/interaction/filter/Elastic.h>
#include <chimp/interaction/filter/Label.h>

#include <physical/physical.h>

#include <iostream>
#include <iterator>


using physical::unit::nm;
using physical::unit::K;
static const double nm2 = nm*nm;

typedef chimp::make_options<>::type
  ::setParticle< simtest::Particle >::type options;


int main() {
  namespace filter = chimp::interaction::filter;
  typedef boost::shared_ptr<filter::Base> SP;
  typedef chimp::RuntimeDB< options > DB;

  DB db;

  /* load information from XML file. */
  db.addParticleType("87Rb");
  db.addParticleType("Ar"  );
  db.addParticleType("e^-" );
  db.addParticleType("Hg"  );
  db.addParticleType("Hg^+");

  db.filter = SP(
                new filter::Or( SP(new filter::Elastic),
                                SP(new filter::Label("inelastic")) )
              );

  /* set up the the runtime database */
  db.initBinaryInteractions();


  /* create all particles */
  std::vector< simtest::Particle > particles;
  simtest::createRandomParticles( particles , n_particles, 300*K, db );

  /* Create the mock cell*/
  Cell cell( particles.begin(), particles.end(), db.getProps().size() );

  /* sort and perform statistical measurements. */
  sortBySpecies( particles.begin(), particles.end(), cell.types );
  doCellMeasurements( cell, db );

  /* spit out known interactions and attempt execution */
  std::cout << "\n\nSimple 0-D test using each interaction set "
               "known to the runtime database:\n";
  std::vector<Particle> result_list;
  CollisionsDriver<>()(
    cell.types.begin(),
    cell.types.end(),
    result_list,
    cell,
    db
  );

  /* lets print out some statistics. */
  // FIXME:  where are we going to get these said statistics???
  std::cout << std::endl;

  return 0;
}

