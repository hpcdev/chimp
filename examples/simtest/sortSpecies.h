#ifndef chimplib_examples_simtest_sortSpecies_h
#define chimplib_examples_simtest_sortSpecies_h

#include <olson-tools/nsort/NSort.h>
#include <olson-tools/nsort/map/type.h>

#include <cassert>

namespace simtest {

  /** Sort only the types in this cell.  It is necessary to have all the types
   * sorted and the types vector set correctly in order to use the interaction
   * stuff correctly.  This is because we will be randomly selecting pairs
   * with specific type values.  This function is nearly a duplicate of the
   * octree::Octree::sortOnlyTypes function of the /dsmc//octree library. 
   */
  template < typename ParticleIterator,
             typename SpeciesRanges >
  void sortBySpecies( const ParticleIterator & pbegin,
                      const ParticleIterator & pend,
                      SpeciesRanges & sranges ) {
    assert( sranges.size() > 0 );

    // Create type sorter
    namespace nsort = olson_tools::nsort;
    nsort::NSort< nsort::map::type > s( sranges.size() );

    // execute sort
    s.sort( pbegin, pend );

    /** Sets each of the ranges for the species. */
    for ( int i = 0; i < static_cast<int>( sranges.size() ); ++i ) {
      typedef typename SpeciesRanges::value_type Range;
      sranges[i] = Range( pbegin + s.begin(i), pbegin + s.end(i) );
    }
  }

}

#endif // chimplib_examples_simtest_sortSpecies_h
