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


#ifndef chimp_examples_simtest_sortSpecies_h
#define chimp_examples_simtest_sortSpecies_h

#include <xylose/nsort/NSort.h>
#include <xylose/nsort/map/species_only.h>

#include <cassert>

namespace simtest {

  /** Sort only the species in this cell.  It is necessary to have all the
   * species sorted and the species vector set correctly in order to use the
   * interaction stuff correctly.  This is because we will be randomly selecting
   * pairs with specific species values.  This function is nearly a duplicate of
   * the octree::Octree::sortOnlyTypes function of the /dsmc//octree library. 
   */
  template < typename ParticleIterator,
             typename SpeciesRanges >
  void sortBySpecies( const ParticleIterator & pbegin,
                      const ParticleIterator & pend,
                      SpeciesRanges & sranges ) {
    assert( sranges.size() > 0 );

    // Create species sorter
    namespace nsort = xylose::nsort;
    nsort::NSort< nsort::map::species_only > s( sranges.size() );

    // execute sort
    s.sort( pbegin, pend );

    /** Sets each of the ranges for the species. */
    for ( int i = 0; i < static_cast<int>( sranges.size() ); ++i ) {
      typedef typename SpeciesRanges::value_type Range;
      sranges[i] = Range( pbegin + s.begin(i), pbegin + s.end(i) );
    }
  }

}

#endif // chimp_examples_simtest_sortSpecies_h
