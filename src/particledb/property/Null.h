
#ifndef particledb_property_Null_h
#define particledb_property_Null_h


#include <string>
#include <ostream>


namespace particledb {
  namespace property {

    /** A simple null property with no storage requirements. */
    template < unsigned int id = 0 >
    struct Null {
      typedef Null super;

      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out;
      }

      template < typename DBnode >
      static inline Null load(const DBnode & x) { return Null(); }
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_Null_h
