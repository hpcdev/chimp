#include "physical_calc.h"
#include <physical/calc/infix.h>

namespace particledb {
    void prepareCalculator(const olson_tools::xml::XMLDoc & doc) {
        /* prepare infix units calculator. */
        using physical::calc::InfixCalc;
        using physical::calc::symbol;
        using olson_tools::xml::XMLContext;
        InfixCalc::base_calc & calc = InfixCalc::instance();
    
        /* clear the old symbols out */
        calc.symbols.clear();
        calc.addMathLib();
        calc.addPhysicalUnits();
    
        XMLContext::set xl = doc.eval("/ParticleDB/*/calc-commands/command");
        XMLContext::set::iterator i = xl.begin();
        for (; i != xl.end(); i++) {
            const XMLContext & x = (*i);
            calc.exec(x.parse<std::string>());
        }
    }
} /* namespace particledb */