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
 * Declaration of generic particle accessor functions such as velocity,
 * position, type, and species.
 */

#ifndef chimp_interaction_ParticleAccessors_h
#define chimp_interaction_ParticleAccessors_h


#include <olson-tools/Vector.h>

namespace chimp {
  namespace interaction {

    using olson_tools::Vector;

    /** Generic NON-const accessor for particle velocity. */
    template < typename ParticleT >
    static Vector<double,3> & velocity( ParticleT & p ) {
      return p.v;
    }

    /** Generic const accessor for particle velocity. */
    template < typename ParticleT >
    static const Vector<double,3> & velocity( const ParticleT & p ) {
      return p.v;
    }

    /** Generic particle velocity set function. */
    template < typename ParticleT >
    static void setVelocity( ParticleT & p, const Vector<double,3> & v ) {
      p.v = v;
    }


    /** Generic NON-const accessor for particle position. */
    template < typename ParticleT >
    static Vector<double,3> & position( ParticleT & p ) {
      return p.x;
    }

    /** Generic const accessor for particle position. */
    template < typename ParticleT >
    static const Vector<double,3> & position( const ParticleT & p ) {
      return p.x;
    }


    /** Generic NON-const accessor for particle species. */
    template < typename ParticleT >
    static int & species( ParticleT & p ) {
      return p.type;
    }

    /** Generic const accessor for particle species. */
    template < typename ParticleT >
    static const int & species( const ParticleT & p ) {
      return p.type;
    }

    /** Generic accessor for particle weight:  defaults to just returning 1. */
    template < typename ParticleT >
    static double weight( const ParticleT & p ) {
      return 1;
    }

  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_ParticleAccessors_h
