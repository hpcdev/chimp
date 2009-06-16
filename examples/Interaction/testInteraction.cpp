#include <iostream>
#include <particledb/interaction/Interaction.h>
#include <particledb/interaction/Set.h>
#include <particledb/ParticleDB.h>
#include <particledb/physical_calc.h>

#ifndef   PARTICLEDB_XML
#  define PARTICLEDB_XML  "particledb.xml"
#endif


int main() {
    using particledb::xml::XMLContext;
    using particledb::interaction::find_all_interactions;
    using particledb::interaction::find_elastic_interactions;
    using particledb::interaction::filter_interactions;
    using particledb::interaction::Output;
    using particledb::interaction::Equation;
    using particledb::interaction::Set;
    using std::string;

    string A = "e^-";
    string B = "Hg";
    Set set;

    using particledb::db;
    particledb::prepareCalculator(db.xmlDb);

    db.addParticleType(A);
    db.addParticleType(B);

    set.lhs.A = db.findParticleIndx(A);
    set.lhs.B = db.findParticleIndx(B);

    db.allowed_equations.insert('('+A+")+("+B+")->("+A+")+("+B+')');

    std::cout << "finding all interactions ..." << std::endl;
    XMLContext::list xl = find_all_interactions(db.xmlDb.root_context, A, B);
    for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
        string Eq = i->query<string>("Eq");

        std::cout << Eq << std::endl;
    }



    std::cout << "testing interaction filter..." << std::endl;
    XMLContext::set xs = filter_interactions(xl, db.allowed_equations);

    for (XMLContext::set::iterator i = xs.begin(); i != xs.end(); i++) {
        string Eq = i->query<string>("Eq");

        std::cout << Eq << std::endl;
        set.rhs.push_back(Equation::load(*i,db));
    }

    for (Equation::list::iterator i = set.rhs.begin(); i!=set.rhs.end(); i++) {
        i->print(std::cout << "Added Eq:  ",db) << std::endl;
    }




    set.rhs.clear();
    std::cout << "finding elastic interactions ..." << std::endl;
    xl = find_elastic_interactions(db.xmlDb.root_context, A, B);
    for (XMLContext::set::iterator i = xl.begin(); i != xl.end(); i++) {
        string Eq = i->query<string>("Eq");

        std::cout << Eq << std::endl;
        set.rhs.push_back(Equation::load(*i,db));
    }

    for (Equation::list::iterator i = set.rhs.begin(); i!=set.rhs.end(); i++) {
        i->print(std::cout << "Added Eq:  ",db) << std::endl;
    }

    return 0;
}
