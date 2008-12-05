/** \file Simple test of particledb::Particle mechanics. */

#include "../physical_calc.h"
#include "VHSCrossSection.h"
#include "DATACrossSection.h"
#include <iostream>
#include <fstream>
#include <olson-tools/physical/physical.h>
#include <stdexcept>

using namespace particledb::xml;
using namespace physical::elements;
using particledb::interaction::CrossSection;
using particledb::interaction::VHSCrossSection;
using particledb::interaction::DATACrossSection;
using physical::constant::amu;
using physical::constant::m_e;
using physical::unit::m;
using physical::unit::nm;
using physical::unit::s;

std::ostream & print(std::ostream & out, CrossSection * cs, const double & v0, const double & v1, const int & N) {
    double dv = (v1-v0) / N;
    const double nm2 = nm*nm;
    for (double v = v0 + 0.5*dv; v <= v1; v += dv) {
        out << v << '\t' << (cs->cross_section(v)/nm2) << '\n';
    }

    return out;
}

const double v0 = 100*m/s;
const double v1 = 10000000*m/s;
const int N = 100;

int main() {

    XMLDoc db("../particledb.xml");
    prepareCalculator(db);

    std::ofstream fvhs("vhs.dat");
    std::ofstream fdata("data.dat");

    std::cout << "testing VHS..." << std::endl;
    XMLContext::list xl = db.root_context.eval("//Interaction[cross_section/@type='vhs/vss']");
    for (XMLContext::list::iterator i = xl.begin(); i!=xl.end(); i++) {
        std::string Eq = i->query<std::string>("Eq");
        std::cout << "Eq:  " << Eq << std::endl;

        XMLContext x = i->find("cross_section");
        VHSCrossSection cs = VHSCrossSection::load(x,0.5*87*amu);
        //cs.print(std::cout) << std::endl;

        fvhs << "# Eq:  " << Eq << '\n';
        print(fvhs, &cs, v0, v1, N) << "\n\n";
    }


    std::cout << "testing DATACrossSection..." << std::endl;
    xl = db.root_context.eval("//Interaction[cross_section/@type='data']");
    for (XMLContext::list::iterator i = xl.begin(); i!=xl.end(); i++) {
        std::string Eq = i->query<std::string>("Eq");
        std::cout << "Eq:  " << Eq << std::endl;

        XMLContext x = i->find("cross_section");
        try {
            DATACrossSection cs = DATACrossSection::load(x,m_e*Hg::mass / (m_e + Hg::mass));
            //cs.print(std::cout) << std::endl;

            fdata << "# Eq:  " << Eq << '\n';
            print(fdata, &cs, v0, v1, N) << "\n\n";
        } catch (std::runtime_error & e) {
            std::cout << e.what();
        }
    }

    fvhs.close();
    fdata.close();

    return 0;
}
