#include <iostream>
#include "Interaction.h"
#include "Set.h"
#include "../ParticleDB.h"
#include "../physical_calc.h"

int main() {
    using particledb::xml::XMLContext;
    using particledb::interaction::find_all_interactions;
    using particledb::interaction::find_elastic_interactions;
    using particledb::interaction::Output;
    using particledb::interaction::Equation;
    using particledb::interaction::Set;
    using std::string;
    using particledb::RuntimeDB;

    string A = "e-";
    string B = "Hg";
    Set set;

    RuntimeDB<> db("../particledb.xml");
    prepareCalculator(db.xmlDb);

    set.lhs.A = db.addParticleType(A);
    try {
        set.lhs.B = db.addParticleType(B);
    } catch (particledb::xml::no_results){
        RuntimeDB<>::prop_type pB;
        pB.name::value = B;
        pB.mass::value = 10;
        set.lhs.B = db.addParticleType(pB);
    }


    std::cout << "finding all interactions ..." << std::endl;
    XMLContext::list xl = find_all_interactions(db.xmlDb.root_context, A, B);
    for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
        string Eq = i->query<string>("Eq");

        std::cout << Eq << std::endl;
    }


    std::cout << "finding elastic interactions ..." << std::endl;
    xl = find_elastic_interactions(db.xmlDb.root_context, A, B);
    for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
        string Eq = i->query<string>("Eq");

        std::cout << Eq << std::endl;
        set.rhs.push_back(Equation::load(*i,db));
    }

    for (Equation::list::iterator i = set.rhs.begin(); i!=set.rhs.end(); i++) {
        i->print(std::cout << "Added Eq:  ",db) << std::endl;
    }

    return 0;
}
