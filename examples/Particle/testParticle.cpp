/** \file Simple test of particledb::Particle mechanics. */

#include <particledb/Particle.h>
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
    particledb::Particle::Properties p = particledb::Particle::Properties::load(x);

    std::cout << "p.name:  "   << p.name::value << std::endl;
    std::cout << "p.size:  "   << p.size::value << std::endl;
    std::cout << "p.mass:  "   << p.mass::value << std::endl;
    std::cout << "p.charge:  " << p.charge::value << std::endl;

    std::cout << '{' << p << '}' << std::endl;

    double bob = 10*physical::unit::m;
    return 0;
}
