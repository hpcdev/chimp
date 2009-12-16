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
 * Template options for the RuntimeDB class.
 * */

#ifndef chimp_make_options_h
#define chimp_make_options_h

#include <chimp/interaction/Particle.h>
#include <chimp/property/DefaultSet.h>

namespace chimp {

  /** Metafunction to generate the chimp::options class.
   * @tparam _Particle
   *   Class that is used for the collision/interaction functions.
   *   The Particle class provides information concerning velocity, position,
   *   and species (though only velocity is used within the library currently).
   *   Access to this data from a Particle instance will be provided using
   *   non-member accessor functions called velocity, position, and species.
   *   Chimp provides default implementations of these that expect that the
   *   Particle class has public 3-vector members x and v and one public member
   *   called 'type' that represents the species of the particle.  <br>
   *   If this does not follow the interface for the Particle class actually
   *   used, then the accessor functions must be overloaded correctly.<br>
   *   <br>
   *   For example, if a particular simulation stores velocities and positions
   *   and types in separate arrays, a proxy Particle class must be used with
   *   correctly overloaded accessors.  For instance:
   *   \verbatim
       /// The global velocity list;
       static Vector<double,3> velocities[10];
       static Vector<double,3> positions[10];
       static int species[10];
       
       struct ParticleProxy {
         int index;
         ParticleProxy( const int & index )
           : index(index) { }
       };
       
       void setVelocity( ParticleProxy & pp, const Vector<double,3> & v ) {
         velocities[pp.index] = v;
       }
       
       const Vector<double,3> & velocity( const ParticleProxy & pp ) {
         return velocities[pp.index];
       }

       // and so forth for position and species functions...
   *   \endverbatim
   *   See the default implementations chimp::interaction::velocity,
   *   chimp::interaction::position, chimp::interaction::species.
   *
   *   <br>
   *   [Default:  chimp::interaction::Particle]<br>
   *
   * @tparam _Properties
   *   Aggregate class of all particle properties that will be loaded from the
   *   xml file.  <br>
   *   [Default:  chimp::property::DefaultSet]
   * */
  template <
    typename _Particle   = chimp::interaction::Particle,
    typename _Properties = chimp::property::DefaultSet
  >
  struct make_options {
    /** The result of the chimp::make_options template metafunction. */
    struct type {
      /** The type of particle that can be used for the collision/interaction
       * functions. */
      typedef _Particle   Particle;
      /** Aggregate class of all particle properties that will be loaded from
       * the xml file. */
      typedef _Properties Properties;

      /** Set options with the given Particle type. */
      template < typename T >
      struct setParticle {
        typedef typename make_options< T, Properties >::type type;
      };/* setParticle */

      /** Set options with the given Particle type. */
      template < typename T >
      struct setProperties {
        typedef typename make_options< Particle, T >::type type;
      };/* setProperties */
    };/* struct type */
  };/* make_options */

}/* namespace chimp */

#endif // chimp_make_options_h
