
#ifndef chimp_property_DefaultSet_h
#define chimp_property_DefaultSet_h


#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/size.h>
#include <chimp/property/mass.h>
#include <chimp/property/charge.h>


namespace chimp {
  namespace property {

    /** Default set of properties. */
    typedef Add<
      property::name,
      property::size,
      property::mass,
      property::charge
    > DefaultSet;

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_DefaultSet_h
