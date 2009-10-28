#ifdef chimplib_examples_simtest_Cell_h
#define chimplib_examples_simtest_Cell_h

#include <olson-tools/upper_triangle.h>
#include <olson-tools/nsort/NSort.h>
#include <olson-tools/nsort/map/type.h>

namespace simtest {

  /** Representative implementation of noce/cell type of information similar to
   * that used in a DSMC simulation.
   */
  template <class ParticleIterator>
  class Cell {
    /* TYPEDEFS */
  public:
    typedef ParticleIterator particle_iter_type;
    typedef olson_tools::upper_triangle<interact_info> InteractionDataTable;
    typedef olson_tools::IteratorRange<ParticleIterator> Range;

    struct TypeData {
      double vav;
      double v2;

      TypeData (const double & vav = 0, const double & v2 = 0)
        : vav(vav), v2(v2) {}
    };

    struct interact_info {
      double max_sigma_v_rel;

      interact_info(const double & m = 0) : max_sigma_v_rel(m) {}
    };


    /* MEMBER STORAGE */
  public:
    /** Beginning/ending iterators of all particles belonging to this cell. */
    Range particles;

    /** Beginning/ending iterators of all particles belonging to each type
     * group.  This vector is of length n_types. */
    std::vector< Range > types;

    std::vector< TypeData > data;                           /* size : n */
    InteractionDataTable interaction_info;       /* size : n*(n+1)/2 */
  private:
    /** The number of types that will be used in this cell. */
    int n_types;


    /* MEMBER FUNCTIONS */
  public:
    /** Constructor specifies the number of types in this Cell. */
    Cell (const int & n =  0) {
      interaction_info.resize(n); /* actually : n*(n+1)/2 */
      types.resize(n);
      data.resize(n);
    }

`   /** Get number of types that this cell has been configured to work with.
     * This function will generally be necessary for compatibility with
     * libraries such /dsmc//octree.  This is because for /dsmc//octree the
     * number of allowed types is set at compile time, not run-time.
     */
    const int & getNumberTypes() const { return n_types; }

    /** Sort only the types in this cell.  It is necessary to have all the types
     * sorted and the types vector set correctly in order to use the interaction
     * stuff correctly.  This is because we will be randomly selecting pairs
     * with specific type values.  This function is nearly a duplicate of the
     * octree::Octree::sortOnlyTypes function of the /dsmc//octree library. 
     */
    void sortOnlyTypes(const int & n_types) {
      // Create type sorter
      namespace nsort = olson_tools::nsort;
      nsort::NSort< nsort::map::type > s(n_types);

      // execute sort
      s.sort( particles.begin(), particles.end() );

      setTypeRanges( particles.begin(), s );
    }

  private:
    /** Sets each of the ranges for the types.  This function is only called
     * for Octree leaf nodes.  This function was borrowed from the /dsmc//octree
     * library from the w_type_fields.h file. */
    template < typename NSort >
    void setTypeRanges( const ParticleIterator & begin, const NSort & s ) {
      for ( int i = 0; i < static_cast<int>(n_types); ++i ) {
        types[i] = Range( begin + s.begin(i), begin + s.end(i) );
      }
    }

  };

}

#endif // chimplib_examples_simtest_Cell_h
