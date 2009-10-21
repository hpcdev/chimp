
#ifndef chimp_property_Null_h
#define chimp_property_Null_h


#include <string>
#include <ostream>


namespace chimp {
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

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_Null_h
