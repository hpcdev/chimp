
#include <iostream>
#include "ParticleDB.h"
#include "physical_calc.h"

int main() {
    using particledb::RuntimeDB;

    RuntimeDB<> db;
    prepareCalculator(db.xmlDb);

    db.addParticleType("87Rb");
    db.addParticleType("Ar");

    std::cout << db[0] << std::endl;
    std::cout << db[1] << std::endl;

    return 0;
}
