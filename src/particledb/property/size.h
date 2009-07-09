
#ifndef particledb_property_size_h
#define particledb_property_size_h


#include <olson-tools/xml/XMLDoc.h>

#include <string>
#include <ostream>


namespace particledb {
  namespace property {

    /** Representative size of this type of particle. */
    struct size {
      /* TYPEDEFS */
      typedef size super;

      /* MEMBER STORAGE */
      double value;

      /* MEMBER FUNCTIONS */
      size(const double & sz = 1) : value(sz) {}
      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out << "size: " << value << sep;
      }
      static inline size load( const xml::XMLContext & x ) { return size(); }
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_size_h
