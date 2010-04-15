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

#ifndef chimp_interaction_selectRandomPair_h
#define chimp_interaction_selectRandomPair_h


#include <chimp/interaction/global_rng.h>

namespace chimp {
  namespace interaction {

    template < typename RandomAccessParticleContainer >
    std::pair<
      typename RandomAccessParticleContainer::iterator,
      typename RandomAccessParticleContainer::iterator
    >
    selectRandomPair( RandomAccessParticleContainer & Aparticles,
                      RandomAccessParticleContainer & Bparticles ) {
      typedef typename RandomAccessParticleContainer::iterator PIter;
      double Asz_m05 = Aparticles.size() * 0.999999;
      double Bsz_m05 = Bparticles.size() * 0.999999;

      /* First pick pA */
      PIter pA = Aparticles.begin()
               + static_cast<int>( Asz_m05 * global_rng.rand() );
      PIter pB = pA;
      PIter Bbegin = Bparticles.begin();

      /* now we pick pB */
      while ( pA == pB )
        pB = Bbegin + static_cast<int>( Bsz_m05 * global_rng.rand() );

      return std::make_pair(pA, pB);
    }

  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_selectRandomPair_h
