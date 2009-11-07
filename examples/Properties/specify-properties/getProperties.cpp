/** \file Example specifying properties to load.
 * In this example, we don't use the default set of particle properties to load
 * from the database, but rather specify a (different) subset of particle
 * properties to load.  We use the chimp::property::Add class to aggregate the
 * properties together.  
 *
 * After creating the aggregate properties type, we use the chimp::make_options
 * template metafunction to instantiate a RuntimeDB that only knows about the
 * name and mass of particles.
 */

#include <chimp/RuntimeDB.h>
#include <chimp/property/Add.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>

#include <iostream>

/* This defines the aggregate particle properties that the database will load
 * from the xml file into memory for each particle type added to the
 * chimp::RuntimeDB class. */
namespace property = chimp::property;
typedef property::Add<
  property::name,
  property::mass
> MyProperties;

/* This defines the options that we need to use for the chimp::RuntimeDB class.
 * Using the ::setProperties metafunction, we can specify the particle
 * properties to load separate from any other template parameters of the
 * chimp::make_options metafunction. 
 */
using chimp::make_options;
typedef make_options<>::type::setProperties< MyProperties >::type options;

int main() {
  const char * Pname = "Ar";

  typedef chimp::RuntimeDB< options > DB;
  DB db;
  db.addParticleType(Pname);

  std::cout << "Accessed with string index, each property individually:\n";
  std::cout << "\tp.name:  "   << db[Pname].name::value << std::endl;
  std::cout << "\tp.mass:  "   << db[Pname].mass::value << std::endl;

  int i = db.findParticleIndx(Pname);
  std::cout << "Accessed with integer index, each property individually:\n";
  std::cout << "\tp.name:  "   << db[i].name::value << std::endl;
  std::cout << "\tp.mass:  "   << db[i].mass::value << std::endl;

  std::cout << "Accessed with const reference:\n";
  const DB::Properties & p = db[Pname];
  std::cout << "Printing all properties by using stream insertion:\n";
  std::cout << "\t{" << p << '}' << std::endl;

  return 0;
}
