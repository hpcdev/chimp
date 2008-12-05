
#include <iostream>
#include "ParticleDB.h"
#include "physical_calc.h"

int main() {
    using particledb::RuntimeDB;

    RuntimeDB<> db;
    prepareCalculator(db.xmlDb);

    db.addParticleType("87Rb");
    db.addParticleType("Ar");
    db.addParticleType("e^-");
    db.addParticleType("Hg");
    db.addParticleType("Hg^+");
    db.allowed_equations.insert("(Ar)+(87Rb)->(Ar)+(87Rb)");
    db.allowed_equations.insert("(e^-)+(Hg)->(e^-)+(Hg^+)");

    db.initInteractions();

    for (int i = 0; i < db.getProps().size(); i++)
        std::cout << db[i] << std::endl;

    for (int i = 0; i < db.getProps().size(); i++)
        for (int j = i; j < db.getProps().size(); j++)
            db(i,j).print(std::cout, db);
    std::cout << std::endl;

    return 0;
}
