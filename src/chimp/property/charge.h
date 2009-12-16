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
 * Definition of charge property. 
 */

#ifndef chimp_property_charge_h
#define chimp_property_charge_h

#include <chimp/property/Generic.h>

namespace chimp {
  namespace property {

    /** Namespace for the properties tag classes that provide the xpath/xml
     * labels/queries. */
    namespace tag {
      /** The tag class for charge. */
      template < unsigned int i = 0>
      struct charge { static const char * label; };

      template < unsigned int i >
      const char * charge<i>::label = "charge";
    }

    /** Struct defs for charge property.  We do this in
     * this manner to allow the user to later reference the various
     * properties by their name, such as:
     * name::value, mass::value, etc.  Otherwise, the user would have
     * to reference the values by P0::value, P1::value, etc. */
    struct charge : Generic<int, tag::charge<> > {
      /** Typedef of derivative of Generic class--required by property::Add. */
      typedef Generic<int, tag::charge<> > super;

      /** Mass property constructor.  By default, this sets charge to 'zero'. */
      charge(const super::value_type & c = super().value) : super(c) {}
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_charge_h
