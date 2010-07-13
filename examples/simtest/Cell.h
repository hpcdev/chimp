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


#ifndef chimp_examples_simtest_Cell_h
#define chimp_examples_simtest_Cell_h

#include "Particle.h"

#include <xylose/Vector.h>
#include <xylose/IteratorRange.h>

namespace simtest {
  using xylose::Vector;

  /** Representative implementation of node/cell type of information similar to
   * that used in a DSMC simulation.
   */
  class Cell {
    /* TYPEDEFS */
  public:
    typedef std::vector<Particle>::iterator ParticleIterator;
    typedef xylose::IteratorRange< ParticleIterator > SpeciesRange;

    struct SpeciesData {
      Vector<double,3> v_min;/**< Average velocity <v>. */
      Vector<double,3> v_max;/**< Average square velocity <v2>. */

      SpeciesData( const Vector<double,3> & v_min = 0.0,
                   const Vector<double,3> & v_max  = 0.0 )
        : v_min(v_min), v_max(v_max) { }
    };


    /* MEMBER STORAGE */
  public:
    /** Beginning/ending iterators of all particles belonging to this cell. */
    SpeciesRange particles;

    /** Beginning/ending iterators of all particles belonging to each species
     * group.  This vector is of length n_species. */
    std::vector< SpeciesRange > species;

    /** Per species statistical data. */
    std::vector< SpeciesData > data;             /* size : n */

  private:
    /** The number of species that will be used in this cell. */
    const size_t n_species;

    /** The volume of the cell. */
    const double vol;


    /* MEMBER FUNCTIONS */
  public:
    /** Constructor specifies the number of species in this Cell. */
    Cell( const ParticleIterator & pbegin,
          const ParticleIterator & pend,
          const size_t & n_species,
          const double & vol )
      : particles(pbegin, pend), n_species(n_species), vol(vol) {
      species.resize(n_species);
      data.resize(n_species);
    }

    /** Get number of species that this cell has been configured to work with.
     * This function will generally be necessary for compatibility with
     * libraries such /dsmc//octree.  This is because for /dsmc//octree the
     * number of allowed species is set at compile time, not run-time.
     */
    const size_t & getNumberOfSpecies() const { return n_species; }

    const SpeciesRange & getSpecies( const unsigned int & A ) const {
      return species[A];
    }

    SpeciesRange & getSpecies( const unsigned int & A ) {
      return species[A];
    }


    /** Determine the maximum relative velocity between two species.
     * Normally, a simulation might maintain this quantity over time and update
     * it periodically rather than calculating it every time at runtime.
     */
    double maxRelativeVelocity( const unsigned int & A, 
                                const unsigned int & B ) const {
      const Vector<double,3> & vA_min = data[A].v_min,
                             & vA_max = data[A].v_max,
                             & vB_min = data[B].v_min,
                             & vB_max = data[B].v_max;

      /* Simple estimator for maximum relative velocity between cross species.*/
      return std::max( (vA_max - vB_min).abs(),
                       (vB_max - vA_min).abs() );
    }

    /** Return the volume of the cell. */
    double volume() const {
      return vol;
    }

  };

}

#endif // chimp_examples_simtest_Cell_h
