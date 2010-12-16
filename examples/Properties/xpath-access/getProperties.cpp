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


/** \file Simple test of chimp::property mechanics. */

#include <chimp/property/DefaultSet.h>
#include <chimp/physical_calc.h>
#include <iostream>
#include <physical/physical.h>

int main() {
  namespace xml = xylose::xml;

  xml::Doc db(#CHIMP_PARTICLEDB_XML);
  chimp::prepareCalculator(db);

  xml::Context x = db.root_context.find("/ParticleDB//particles/Particle[@name='87Rb']");
  chimp::property::DefaultSet p = chimp::property::DefaultSet::load(x);

  std::cout << "Accessing each property individually:\n";
  std::cout << "p.name:  "   << p.name::value << std::endl;
  std::cout << "p.mass:  "   << p.mass::value << std::endl;
  std::cout << "p.charge:  " << p.charge::value << std::endl;

  std::cout << "Printing all properties by using stream insertion:\n";
  std::cout << "\t{" << p << '}' << std::endl;

  return 0;
}
