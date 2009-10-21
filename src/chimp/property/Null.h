
#ifndef chimp_property_Null_h
#define chimp_property_Null_h


#include <string>
#include <ostream>


namespace chimp {
  namespace property {

    /** A simple null property with no storage requirements. */
    template < unsigned int id = 0 >
    struct Null {
      /** An alias to the Null type itsself; required by property::Add
       * template metafunction. */
      typedef Null super;

      /** A NO-OP function. */
      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out;
      }

      /** Calls the default NO-OP constructor to create the zero-storage Null
       * class. */
      template < typename DBnode >
      static inline Null load(const DBnode & x) { return Null(); }
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_Null_h
