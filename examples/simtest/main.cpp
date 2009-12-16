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



#include "Cell.h"
#include "Particle.h"
#include "sortSpecies.h"
#include "cellMeasurements.h"

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/filter/Or.h>
#include <chimp/interaction/filter/Elastic.h>
#include <chimp/interaction/filter/Label.h>

#include <physical/physical.h>

#include <iostream>
#include <iterator>


using simtest::Cell;
using simtest::createRandomParticles;
using simtest::sortBySpecies;
using simtest::doCellMeasurements;

using physical::unit::nm;
using physical::unit::K;
static const double nm2 = nm*nm;

typedef chimp::make_options<>::type
  ::setParticle< simtest::Particle >::type options;

const int n_particles = 100;

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
                new filter::Or(
                  SP(new filter::Elastic),
                  SP(new filter::Label("inelastic"))
                )
              );

  /* set up the the runtime database */
  db.initBinaryInteractions();


  /* create all particles */
  std::vector< simtest::Particle > particles;
  createRandomParticles( particles , n_particles, 300*K, db );

  /* Create the mock cell*/
  Cell cell( particles.begin(), particles.end(), db.getProps().size() );

  /* sort and perform statistical measurements. */
  sortBySpecies( particles.begin(), particles.end(), cell.types );
  doCellMeasurements( cell, db );

  /* spit out known interactions and attempt execution */
  std::cout << "\n\nSimple 0-D test using each interaction set "
               "known to the runtime database:\n";
  std::vector< simtest::Particle > result_list;
  // FIXME:  Finish the implementation of the collision driver
  //CollisionsDriver<>()(
  //  cell.types.begin(),
  //  cell.types.end(),
  //  result_list,
  //  cell,
  //  db
  //);

  /* lets print out some statistics. */
  // FIXME:  where are we going to get these said statistics???
  std::cout << std::endl;

  return 0;
}

