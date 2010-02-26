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


/** \file Example specifying properties to load.
 * In this example, we don't use the default set of particle properties to load
 * from the database, but rather specify a (different) subset of particle
 * properties to load.  We use the chimp::property::MakeList class to aggregate the
 * properties together.  
 *
 * After creating the aggregate properties type, we use the chimp::make_options
 * template metafunction to instantiate a RuntimeDB that only knows about the
 * name and mass of particles.
 */

#include <chimp/RuntimeDB.h>
#include <chimp/property/list.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>
#include <chimp/property/DefaultSet.h>
#include <chimp/property/define.h>

#include <physical/runtime.h>

#include <iostream>

/* some shortcuts to some useful namespaces for this example. */
namespace dim = runtime::physical::dimension;
namespace property = chimp::property;


/* create a new property type that is only in the local dataset and is a
 * unitless quantity... */

/* This defines a new particle property that is unitless, comes from the
 * xml-node 'niceinfo' directly within the particle node, and has a default
 * value of -19 if it is missing.  
 *
 * Defining properties in this manner AUTOMAGICALLY creates a property that
 * 1) already knows how to extract itself out of the xml,
 * 2) checks the extacted information for the specified physical units (dimensional analysis),
 * and 3) assigns a default value if niceinfo did not exist. 
 *
 *                                name    type    units(less)   xpath   default */
CHIMP_DEFINE_PARTICLE_PROPERTY( niceness, double, dim::unity, "niceinfo", -19 );


/* This defines a property in a very similar manner to niceness, except that it
 * is required to exist for each set of particle properties loaded into memory. */
CHIMP_DEFINE_REQUIRED_PARTICLE_PROPERTY( meanness, double, dim::viscosity, "meaninfo" );


/* This defines the aggregate particle properties that the database will load
 * from the xml file into memory for each particle type added to the
 * chimp::RuntimeDB class.  For this example, we simply append the niceness
 * property to the default set of properties (as defined in chimp).  The order
 * within MakeList is not important.  */
typedef property::MakeList<
  property::DefaultSet,
  niceness
>::type MyProperties;


/* You could also define the list entirely, rather than just concatenating more
 * items onto the default list. */
typedef property::MakeList<
  property::name,
  property::mass,
  niceness,
  meanness
>::type MyProperties2;

/* This defines the options that we need to use for the chimp::RuntimeDB class.
 * Using the ::setProperties metafunction, we can specify the particle
 * properties to load separate from any other template parameters of the
 * chimp::make_options metafunction. 
 */
using chimp::make_options;
typedef make_options<>::type::setProperties< MyProperties >::type options;

int main() {
  const char * Pname = "Ar";

  typedef chimp::RuntimeDB< options > DB;
  DB db;
  db.addParticleType(Pname);

  std::cout << "Accessed with string index, each property individually:\n";
  std::cout << "\tp.name    :  "   << db[Pname].name::value << std::endl;
  std::cout << "\tp.mass    :  "   << db[Pname].mass::value << std::endl;
  std::cout << "\tp.niceness:  "   << db[Pname].niceness::value << std::endl;

  int i = db.findParticleIndx(Pname);
  std::cout << "Accessed with integer index, each property individually:\n";
  std::cout << "\tp.name    :  "   << db[i].name::value << std::endl;
  std::cout << "\tp.mass    :  "   << db[i].mass::value << std::endl;
  std::cout << "\tp.niceness:  "   << db[i].niceness::value << std::endl;

  std::cout << "Accessed with const reference:\n";
  const DB::Properties & p = db[Pname];
  std::cout << "Printing all properties by using stream insertion:\n";
  std::cout << "\t{" << p << '}' << std::endl;

  return 0;
}
