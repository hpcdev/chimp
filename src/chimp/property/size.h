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
 * Definition of size property. 
 */

#ifndef chimp_property_size_h
#define chimp_property_size_h


#include <xylose/xml/Doc.h>

#include <string>
#include <ostream>


namespace chimp {
  namespace xml = xylose::xml;

  namespace property {

    /** Representative size of this species of particle. */
    struct size {
      /* TYPEDEFS */
      typedef size super;

      /* MEMBER STORAGE */
      double value;

      /* MEMBER FUNCTIONS */
      /** Constructor. */
      size(const double & sz = 1) : value(sz) {}

      /** Stream printer. */
      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out << "size: " << value << sep;
      }

      /** Load function (does not use xml; returns default size value. */
      static inline size load( const xml::Context & x ) { return size(); }
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_size_h
