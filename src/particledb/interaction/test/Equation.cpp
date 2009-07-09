/** \file
 * Test file for the  Equation class.
 * */
#define BOOST_TEST_MODULE  Equation


#ifndef   PARTICLEDB_XML
#  define PARTICLEDB_XML  "file.xml"
#endif

#include <particledb/interaction/Equation.h>
#include <particledb/interaction/CrossSection.h>
#include <particledb/interaction/VHSCrossSection.h>
#include <particledb/interaction/DATACrossSection.h>
#include <particledb/property/DefaultSet.h>


#include <olson-tools/xml/XMLDoc.h>

#include <physical/calc/symbol.h>
#include <physical/calc/infix.h>

#include <boost/shared_ptr.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <map>


namespace {

  using olson_tools::xml::XMLDoc;
  using olson_tools::xml::XMLContext;

  void prepareCalculator(const XMLDoc & doc) {
    /* prepare infix units calculator. */
    using runtime::physical::calc::InfixCalc;
    using runtime::physical::calc::symbol;
    InfixCalc::base_calc & calc = InfixCalc::instance();

    /* clear the old symbols out */
    calc.symbols.clear();
    calc.addMathLib();
    calc.addPhysicalUnits();

    XMLContext::list xl = doc.eval("//calc-commands/command");
    XMLContext::list::iterator i = xl.begin();
    for (; i != xl.end(); i++) {
      const XMLContext & x = (*i);
      calc.exec(x.parse<std::string>());
    }
  }


  using boost::shared_ptr;
  using particledb::interaction::CrossSection;
  using particledb::interaction::VHSCrossSection;
  using particledb::interaction::DATACrossSection;

  using std::map;
  using std::string;

  struct FakeRuntimeDB {
    /* TYEPDEFS */
    typedef map< string, shared_ptr<CrossSection> > CrossSectionRegistry;
    typedef particledb::property::DefaultSet Properties;
    typedef map< string, std::pair<Properties, int> > prop_map;

    /* MEMBER STORAGE */
    prop_map * types_ptr;

    CrossSectionRegistry cross_section_registry;

    /* MEMBER FUNCTIONS */
    FakeRuntimeDB() {
      cross_section_registry["vhs"].reset(new VHSCrossSection);
      cross_section_registry["data"].reset(new DATACrossSection);

      types_ptr = new prop_map;
    }
    ~FakeRuntimeDB() {
      delete types_ptr;
    }

    int findParticleIndx( const string & n ) const {
      prop_map & types = *types_ptr;

      int i = types.size();
      Properties p;
      p.name::value = n;
      p.mass::value = 10;
      if ( types.find(n) == types.end() )
        types[n] = std::make_pair(p, i) ;

      return types[n].second;
    }

    const Properties & operator[] (const int & i) const {
      prop_map::iterator j = types_ptr->begin();
      for (int m = 0; m != i && j != types_ptr->end(); ++j, ++m);
      return j->second.first;
    }

    const Properties & operator[] (const std::string & n) const {
      prop_map & types = *types_ptr;

      int i = types.size();
      Properties p;
      p.name::value = n;
      p.mass::value = 10;
      if ( types.find(n) == types.end() )
        types[n] = std::make_pair(p, i) ;

      return types[n].first;
    }
  };

}


BOOST_AUTO_TEST_SUITE( Equation_tests ); // {

  BOOST_AUTO_TEST_CASE( loading ) {
    using olson_tools::xml::XMLDoc;
    XMLDoc xmldb(PARTICLEDB_XML);
    prepareCalculator(xmldb);

    FakeRuntimeDB db;

    using particledb::interaction::Equation;
    Equation eq = Equation::load(xmldb.root_context, "(87Rb)->(87Rb)", db);

    std::ostringstream sstr;
    BOOST_CHECK_EQUAL( static_cast<std::ostringstream&>(eq.print(sstr, db)).str(), "(87Rb)->(87Rb)");

  }

BOOST_AUTO_TEST_SUITE_END(); // }
