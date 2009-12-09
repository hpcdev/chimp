/** \file Simple test of chimp::property mechanics. */

#include <chimp/property/DefaultSet.h>
#include <chimp/physical_calc.h>
#include <iostream>
#include <physical/physical.h>

int main() {
  namespace xml = olson_tools::xml;

  xml::Doc db(PARTICLEDB_XML);
  chimp::prepareCalculator(db);

  xml::Context x = db.root_context.find("/ParticleDB//particles/Particle[@name='87Rb']");
  chimp::property::DefaultSet p = chimp::property::DefaultSet::load(x);

  std::cout << "Accessing each property individually:\n";
  std::cout << "p.name:  "   << p.name::value << std::endl;
  std::cout << "p.mass:  "   << p.mass::value << std::endl;
  std::cout << "p.charge:  " << p.charge::value << std::endl;

  std::cout << "Printing all properties by using stream insertion:\n";
  std::cout << "\t{" << p << '}' << std::endl;

  return 0;
}
