
#ifndef particledb_property_size_h
#define particledb_property_size_h


#include <olson-tools/xml/Doc.h>

#include <string>
#include <ostream>


namespace particledb {
  namespace xml = olson_tools::xml;

  namespace property {

    /** Representative size of this type of particle. */
    struct size {
      /* TYPEDEFS */
      typedef size super;

      /* MEMBER STORAGE */
      double value;

      /* MEMBER FUNCTIONS */
      /** Constructor. */
      size(const double & sz = 1) : value(sz) {}

      /** Stream printer. */
      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out << "size: " << value << sep;
      }

      /** Load function (does not use xml; returns default size value. */
      static inline size load( const xml::Context & x ) { return size(); }
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_size_h
