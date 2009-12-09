/** \file
 * Definition of the default set of properties to load from database.
 */

#ifndef chimp_property_DefaultSet_h
#define chimp_property_DefaultSet_h


#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>
#include <chimp/property/charge.h>


namespace chimp {
  namespace property {

    /** The default set of particle properties includes:  name, mass, and
     * charge. */
    typedef Add<
      property::name,
      property::mass,
      property::charge
    > DefaultSet;

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_DefaultSet_h
