/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Copyright (C) 2006, 2008 Spencer E. Olson                                   *
 *                                                                             *
 * This file is part of CHIMP                                                  *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


#include <chimp/physical_calc.h>

#include <physical/calc/infix.h>

namespace chimp {
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
} /* namespace chimp */
