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



namespace simtest {

  template < typename RandomAccessParticleContainer >
  std::pair<
    typename RandomAccessParticleContainer::iterator,
    typename RandomAccessParticleContainer::iterator
  >
  selectRandomPair( RandomAccessParticleContainer & Aparticles,
                    RandomAccessParticleContainer & Bparticles ) {
    /* Pick the particles that need to interact. */
    Particle::list::iterator pi
         = Aparticles.begin() + int(Aparticles.size()*MTRNGrand()*0.99999999);

    Particle::list::iterator pj = pi;

    while ( pi == pj )
      pj = BParticles.begin() + int(BParticles.size()*MTRNGrand()*0.99999999);

    return std::make_pair(pi, pj);
  }

}

