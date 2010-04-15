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
 * position, and species.
 */

#ifndef chimp_accessors_h
#define chimp_accessors_h


#include <xylose/Vector.h>

namespace chimp {
  namespace accessors {
    namespace particle {

      using xylose::Vector;

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
        return p.species;
      }

      /** Generic const accessor for particle species. */
      template < typename ParticleT >
      static const int & species( const ParticleT & p ) {
        return p.species;
      }


      /** Generic NON-CONST accessor for particle weight. */
      template < typename ParticleT >
      static float & weight( ParticleT & p ) {
        return p.weight;
      }

      /** Generic CONST accessor for particle weight. */
      template < typename ParticleT >
      static const float & weight( const ParticleT & p ) {
        return p.weight;
      }

    } /* namespace chimp::accessors::particle */
  } /* namespace chimp::accessors */
} /* namespace chimp */

#endif // chimp_accessors_h
