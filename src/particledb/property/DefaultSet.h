
#ifndef particledb_property_DefaultSet_h
#define particledb_property_DefaultSet_h


#include <particledb/property/Add.h>
#include <particledb/property/name.h>
#include <particledb/property/size.h>
#include <particledb/property/mass.h>
#include <particledb/property/charge.h>


namespace particledb {
  namespace property {

    /** Default set of properties. */
    typedef Add<
      property::name,
      property::size,
      property::mass,
      property::charge
    > DefaultSet;

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_DefaultSet_h
