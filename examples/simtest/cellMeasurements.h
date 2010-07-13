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


#ifndef chimp_examples_simtest_cellMeasurements_h
#define chimp_examples_simtest_cellMeasurements_h

#include "Cell.h"

#include <chimp/accessors.h>

#include <xylose/Vector.h>
#include <xylose/power.h>

#include <cassert>

namespace simtest {

  /** Perform statistical measurements relevant to a cell. */
  template < typename RnDB >
  void doCellMeasurements( Cell & cell, const RnDB & db ) {
    using chimp::accessors::particle::velocity; // generic accessor for velocity

    const int n = cell.species.size();
    assert( cell.species.size() > 0 );
    assert( cell.species.size() == cell.data.size() );

    /* measure the single species data:  v_max, v_min. */
    for ( int i = 0; i < n; ++i ) {
      Cell::ParticleIterator pi   = cell.species[i].begin();
      Cell::ParticleIterator pend = cell.species[i].end();

      /* velocity moments of the distribution */
      Vector<double,3> & v_min = cell.data[i].v_min,
                       & v_max = cell.data[i].v_max;
      v_min = v_max = 0.0;

      for ( ; pi != pend; ++pi ) {
        const Vector<double,3> & pv = velocity( *pi );
        for ( unsigned int k = 0; k < pv.length; ++k ) {
          v_min[k] = std::min(v_min[k], pv[k]);
          v_max[k] = std::max(v_max[k], pv[k]);
        }
      }
    }
  }

}/* namespace simtest */

#endif // chimp_examples_simtest_cellMeasurements_h
