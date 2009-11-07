/** \file Simple test of chimp::Particle mechanics. */

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/ReducedMass.h>
#include <chimp/interaction/cross_section/VHS.h>
#include <chimp/interaction/cross_section/DATA.h>

#include <physical/physical.h>

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace xml = olson_tools::xml;
using namespace physical::elements;
namespace XS = chimp::interaction::cross_section;
using physical::system::si;
const double amu = physical::constant::amu<si>::value;
const double m_e = physical::constant::m_e<si>::value;
using physical::unit::m;
using physical::unit::nm;
using physical::unit::s;

template < typename T >
std::ostream & print( std::ostream & out,
                      XS::Base<T> & cs,
                      const double & v0,
                      const double & v1,
                      const int & N_points ) {
  double dv = (v1-v0) / N_points;
  const double nm2 = nm*nm;
  for (double v = v0 + 0.5*dv; v <= v1; v += dv) {
      out << v << '\t' << ( cs(v) / nm2 ) << '\n';
  }

  return out;
}

const double v0 = 100*m/s;
const double v1 = 10000000*m/s;
const int N_points = 100;

int main() {
    typedef chimp::RuntimeDB<> DB;
    typedef chimp::interaction::ReducedMass Mu;
    DB db;

    std::ofstream fvhs("vhs.dat");
    std::ofstream fdata("data.dat");

    std::cout << "testing VHS..." << std::endl;
    xml::Context::list xl = db.xmlDb.eval("//Interaction[cross_section/@model='vhs']");
    for (xml::Context::list::iterator i = xl.begin(); i!=xl.end(); i++) {
        std::string Eq = i->query<std::string>("Eq");
        std::cout << "Eq:  " << Eq << std::endl;

        xml::Context x = i->find("cross_section");
        XS::VHS<DB::options> cs( x, Mu(87*amu, 87*amu) );
        //cs.print(std::cout) << std::endl;

        fvhs << "# Eq:  " << Eq << '\n';
        print(fvhs, cs, v0, v1, N_points) << "\n\n";
    }


    std::cout << "testing XS::DATA..." << std::endl;
    xl = db.xmlDb.eval("//Interaction[cross_section/@model='data']");
    for (xml::Context::list::iterator i = xl.begin(); i!=xl.end(); i++) {
        std::string Eq = i->query<std::string>("Eq");
        std::cout << "Eq:  " << Eq << std::endl;

        xml::Context x = i->find("cross_section");
        try {
            XS::DATA<DB::options> cs( x, Mu(m_e, Hg::mass) );
            //cs.print(std::cout) << std::endl;

            fdata << "# Eq:  " << Eq << '\n';
            print(fdata, cs, v0, v1, N_points) << "\n\n";
        } catch (std::runtime_error & e) {
            std::cout << e.what();
        }
    }

    fvhs.close();
    fdata.close();

    return 0;
}
