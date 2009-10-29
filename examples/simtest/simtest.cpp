
#include "Particle.h"
#include "cell_info.h"

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/filter/Or.h>
#include <chimp/interaction/filter/Elastic.h>
#include <chimp/interaction/filter/Label.h>

#include <physical/physical.h>

#include <iostream>
#include <iterator>


using physical::unit::nm;
static const double nm2 = nm*nm;



int main() {
  namespace filter = chimp::interaction::filter;
  typedef boost::shared_ptr<filter::Base> SP;
  typedef chimp::RuntimeDB<> DB;

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


  /* Create the mock cell with the given number of particles, uniformly
   * distributed accross each species */
  Cell cell( 10/* particles */, db );


  /* spit out known interactions and attempt execution */
  std::cout << "\n\nSimple 0-D test using each interaction set "
               "known to the runtime database:\n";
  std::vector<Particle> result_list;
  chimp::interaction::Driver<>()(
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
