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


namespace chimp {
  namespace interaction {

    template <
      typename RandomAccessParticleContainer,
      typename RNG
    >
    inline
    std::pair<
      typename RandomAccessParticleContainer::iterator,
      typename RandomAccessParticleContainer::iterator
    >
    selectRandomPair( RandomAccessParticleContainer & Aparticles,
                      RandomAccessParticleContainer & Bparticles,
                      RNG & rng ) {
      typedef typename RandomAccessParticleContainer::iterator PIter;
      const unsigned int Asz = Aparticles.size();
      const unsigned int Bsz = Bparticles.size();

      /* First pick pA */
      PIter pA = Aparticles.begin()
               + static_cast<int>( Asz * rng.randExc() );
      PIter pB = pA;
      PIter Bbegin = Bparticles.begin();

      /* now we pick pB */
      while ( pA == pB )
        pB = Bbegin + static_cast<int>( Bsz * rng.randExc() );

      return std::make_pair(pA, pB);
    }

  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_selectRandomPair_h
