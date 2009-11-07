/** \file
 * Example of accessing the data to create plots for all cross sections
 * pertaining to two common inputs.
 * */

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/filter/Null.h>
#include <chimp/interaction/cross_section/Base.h>

#include <physical/physical.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>

template < typename T >
std::ostream & print( std::ostream & out,
                      chimp::interaction::cross_section::Base<T> & cs,
                      const double & v0,
                      const double & v1,
                      const int & N_points ) {
  using physical::unit::nm;
  double dv = (v1-v0) / N_points;
  const double nm2 = nm*nm;
  for (double v = v0 + 0.5*dv; v <= v1; v += dv) {
      out << v << '\t' << ( cs(v) / nm2 ) << '\n';
  }

  return out;
}

using physical::unit::m;
using physical::unit::s;
static double vi = 100*m/s;
static double vf = 10000000*m/s;
static int N_points = 100;

void usage( const char * prog ) {
  std::cout << "usage:  "
            << prog << " [v_start:v_end [N_points]]\n"
               "\t     v_start:\tBeginning velocity in SI units\n"
               "\t             \t[Default:  "<< vi << " m/s]\n"
               "\t       v_end:\tEnding velocity in SI units\n"
               "\t             \t[Default:  "<< vf << " m/s]\n"
               "\t    N_points:\tNumber of data points\n"
               "\t             \t[Default:  "<< N_points << "]\n"
            << std::endl;
}

int main(int argc, char * argv[]) {
  if ( argc > 1 ) {
    std::istringstream istr( argv[3] );
    char colon = '\0';
    istr >> vi >> colon >> vf;

    if ( colon != ':' || istr.fail() ) {
      std::cout << "could not interpret velocity range." << std::endl;
      usage( argv[0] );
      return EXIT_FAILURE;
    }
  }

  if ( argc > 2 ) {
    std::istringstream istr( argv[4] );
    istr >> N_points;

    if ( istr.fail() ) {
      std::cout << "could not interpret number of points." << std::endl;
      usage( argv[0] );
      return EXIT_FAILURE;
    }
  }
  /* END ARGS SETUP */



  
  std::ofstream fout("data.dat");

  typedef chimp::RuntimeDB<> DB;
  namespace filter = chimp::interaction::filter;
  namespace xml = chimp::xml;

  DB db;
  db.filter.reset( new filter::Null );

  xml::Context::list particles = chimp::getAllParticlesCtx( db.xmlDb );

  db.addParticleType( particles.begin(), particles.end() );

  db.initBinaryInteractions();

  typedef DB::InteractionTable::const_iterator CIter;
  CIter end = db.getInteractions().end();
  for ( CIter i = db.getInteractions().begin(); i != end; ++i ) {

    DB::Set const & set = *i;

    typedef DB::Set::Equation::list::const_iterator EIter;

    for ( EIter i = set.rhs.begin(); i != set.rhs.end(); ++i ) {
      i->print( std::cout << "# Eq: ", db ) << '\n';
      i->print(      fout << "# Eq: ", db ) << '\n';
      print( fout, *(i->cs), vi, vf, N_points ) << "\n\n";
    }

  }

  fout.close();

}

