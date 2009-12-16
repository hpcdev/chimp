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
 * Definition of Null property (represents no real property). 
 */

#ifndef chimp_property_Null_h
#define chimp_property_Null_h


#include <string>
#include <ostream>


namespace chimp {
  namespace property {

    /** A simple null property with no storage requirements. */
    template < unsigned int id = 0 >
    struct Null {
      /** An alias to the Null type itsself; required by property::Add
       * template metafunction. */
      typedef Null super;

      /** A NO-OP function. */
      std::ostream & print( std::ostream & out,
                            const std::string & sep ) const {
        return out;
      }

      /** Calls the default NO-OP constructor to create the zero-storage Null
       * class. */
      template < typename DBnode >
      static inline Null load(const DBnode & x) { return Null(); }
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_Null_h
