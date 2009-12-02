#ifndef chimplib_examples_simtest_cellMeasurements_h
#define chimplib_examples_simtest_cellMeasurements_h

#include "Cell.h"

#include <chimp/interaction/ParticleAccessors.h>

#include <olson-tools/Vector.h>
#include <olson-tools/power.h>

#include <cassert>

namespace simtest {

  /** Perform statistical measurements relevant to a cell. */
  template < typename RnDB >
  void doCellMeasurements( Cell & cell, const RnDB & db ) {
    using chimp::interaction::velocity; // generic accessor for velocity

    const int n = cell.types.size();
    assert( cell.types.size() > 0 );
    assert( cell.types.size() == cell.data.size() );

    /* measure the single species data:  v_max, v_min. */
    for ( int i = 0; i < n; ++i ) {
      Cell::ParticleIterator pi   = cell.types[i].begin();
      Cell::ParticleIterator pend = cell.types[i].end();

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

#endif // chimplib_examples_simtest_cellMeasurements_h
