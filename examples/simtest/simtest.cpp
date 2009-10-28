
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
  using chimp::interaction::stddev_v_rel;
  using physical::unit::m;
  using physical::unit::s;
  using physical::unit::uK;

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
  /* close the xml-file and free associated resources. */
  db.xmlDb.close();


  /* Create the mock cell with the given number of particles, uniformly
   * distributed accross each species */
  Cell cell( 10/* particles */, db );


  /* spit out known interactions and attempt execution */
  std::cout << "\n\nAll known interactions and a small calculateOutPath test:\n";
  typedef DB::InteractionTable::const_iterator CIter;
  for ( CIter i  = db.getInteractions().begin();
              i != db.getInteractions().end(); ++i ) {
    const int & A = i->lhs.A;
    const int & B = i->lhs.B;

    if (i->rhs.size() == 0)
        continue;

    /* now let's try and pick a mock collision pair for this
     * interaction. */
    if ((A == B && cell.types[A].size() < 2) ||
        (cell.types[A].size() == 0 || cell.types[B].size() == 0))
        continue;

    Particle::list::iterator pi
      = cell.types[A].begin() + int(cell.types[A].size()*MTRNGrand()*0.99999999);
    Particle::list::iterator pj = pi;
    while ( pi == pj )
      pj = cell.types[B].begin()
         + int(cell.types[B].size()*MTRNGrand()*0.99999999);

    double v_rel = (pi->v - pj->v).abs();

    double & m_s_v = cell.interaction_info(A,B).max_sigma_v_rel;

    /* This calculates the proper output path, observing both absolute
     * probability that any interaction occurs as well as relative probabilities
     * for when an interaction does occur.  
     *
     * The .first<int> component is the interaction index, such as
     * i->rhs[path.first] to obtain the Equation instance for the interaction
     * that should occur.  
     * The .second<double> component is the cross section value of this
     * particular interaction for the given relative velocity.   */
    std::pair<int,double> path = i->calculateOutPath(m_s_v, v_rel);

    i->print(std::cout << "Performing test interactions for:", db) << '\n';
    if ( path.first < 0 )
      std::cout << "\tNo interaction occurred\n";
    else {
        i->rhs[path.first].print(
            std::cout << "-----\tinputs = {" << (*pi) << ',' << (*pj) << "}\n"
                      << "-----\tmax_sigma_v  :  " << m_s_v << "\n"
                      << "-----\tv_rel        :  " << v_rel << "\n"
                      << "-----\tout path     :  ", db) << "\n"
                         "-----\tcross section:  " << (path.second/nm2) << " nm^2\n";

      /* and now do the interaction for the two different particles.  */
    }
  }

  std::cout << std::endl;

  return 0;
}
