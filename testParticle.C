/** \file Simple test of particledb::Particle mechanics. */

#include "Particle.h"
#include "physical_calc.h"
#include <iostream>
#include <olson-tools/physical/physical.h>

int main() {
    using namespace particledb::xml;

    XMLDoc db("particledb.xml");
    prepareCalculator(db);

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
