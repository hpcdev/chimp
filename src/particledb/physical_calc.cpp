#include <particledb/physical_calc.h>

#include <physical/calc/infix.h>

namespace particledb {
  void prepareCalculator(const xml::Doc & doc) {
    /* prepare infix units calculator. */
    using runtime::physical::calc::InfixCalc;
    using runtime::physical::calc::symbol;
    InfixCalc::base_calc & calc = InfixCalc::instance();
  
    /* clear the old symbols out */
    calc.symbols.clear();
    calc.addMathLib();
    calc.addPhysicalUnits();
  
    xml::Context::list xl = doc.eval("/ParticleDB/*/calc-commands/command");
    xml::Context::list::iterator i = xl.begin();
    for (; i != xl.end(); i++) {
      const xml::Context & x = (*i);
      calc.exec(x.parse<std::string>());
    }
  }
} /* namespace particledb */
