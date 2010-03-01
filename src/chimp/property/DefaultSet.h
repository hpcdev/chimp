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


/** \file
 * Definition of the default set of properties to load from database.
 */

#ifndef chimp_property_DefaultSet_h
#define chimp_property_DefaultSet_h


#include <chimp/property/aggregate.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>
#include <chimp/property/charge.h>
#include <chimp/property/polarizability.h>


namespace chimp {
  namespace property {

    /** The default set of particle properties includes:  name, mass, and
     * charge. */
    typedef Aggregate<
      property::name,
      property::mass,
      property::charge,
      property::polarizability
    >::type DefaultSet;

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_DefaultSet_h
