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
 * Definition of ReducedMass class.
 */

#ifndef chimp_interaction_ReducedMass_h
#define chimp_interaction_ReducedMass_h

#include <chimp/interaction/Input.h>
#include <chimp/property/mass.h>

#include <limits>

namespace chimp {
  namespace interaction {

    /** Simple class to calculate, cache, and provide access to reduced mass and
     * associated values. */
    struct ReducedMass {
      /** The reduced mass. */
      double value;

      /** The reduced mass over the mass of the first particle. */
      double over_m1;

      /** The reduced mass over the mass of the second particle. */
      double over_m2;

      /** Default constructor for ReducedMass sets all quantities to inf. */
      ReducedMass()
        : value  (std::numeric_limits<double>::infinity()),
          over_m1(std::numeric_limits<double>::infinity()),
          over_m2(std::numeric_limits<double>::infinity()) { }

      /** Construct reduced mass by specifying the two relevant masses.*/
      ReducedMass( const double & m1, const double & m2 )
        : value  ( m1 * m2 / ( m1 + m2 ) ),
          over_m1( 1./( 1. + m1/m2 ) ),
          over_m2( 1./( 1. + m2/m1 ) ) { }

      /** Construct reduced mass by specifying the equation input. */
      template < typename RnDB >
      ReducedMass( const interaction::Input & in, const RnDB & db ) {
        using property::mass;
        const double & m1 = db[in.A.species].mass::value;
        const double & m2 = db[in.B.species].mass::value;

        value   = m1 * m2 / ( m1 + m2 );
        over_m1 = 1./( 1. + m1/m2 );
        over_m2 = 1./( 1. + m2/m1 );
      }
    };

  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_ReducedMass_h
