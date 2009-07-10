/** \file Simple test of particledb::property mechanics. */

#include <particledb/property/DefaultSet.h>
#include <particledb/physical_calc.h>
#include <iostream>
#include <physical/physical.h>

#ifndef   PARTICLEDB_XML
#  define PARTICLEDB_XML  "particledb.xml"
#endif

int main() {
    using namespace olson_tools::xml;

    XMLDoc db(PARTICLEDB_XML);
    particledb::prepareCalculator(db);

    XMLContext x = db.root_context.find("/ParticleDB//particles/Particle[@name='87Rb']");
    particledb::property::DefaultSet p = particledb::property::DefaultSet::load(x);

    std::cout << "p.name:  "   << p.name::value << std::endl;
    std::cout << "p.size:  "   << p.size::value << std::endl;
    std::cout << "p.mass:  "   << p.mass::value << std::endl;
    std::cout << "p.charge:  " << p.charge::value << std::endl;

    std::cout << '{' << p << '}' << std::endl;

    return 0;
}
