// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Olson
 *
 */

#ifndef chimp_make_options_h
#define chimp_make_options_h

#include <chimp/interaction/Particle.h>
#include <chimp/property/DefaultSet.h>

namespace chimp {

  /** Metafunction to generate the chimp::options class.
   * @param _Particle
   *   Class that is used for the collision/interaction functions.
   *
   * @param _Properties
   *   Aggregate class of all particle properties that will be loaded from the
   *   xml file.  
   * */
  template <
    typename _Particle   = chimp::interaction::Particle,
    typename _Properties = chimp::property::DefaultSet
  >
  struct make_options {
    struct type {
      typedef _Particle   Particle;
      typedef _Properties Properties;

      /** Set options with the given Particle type. */
      template < typename T >
      struct setParticle {
        typedef typename make_options<
          T,
          Properties
        >::type type;
      };/* setParticle */

      /** Set options with the given Particle type. */
      template < typename T >
      struct setProperties {
        typedef typename make_options<
          Particle,
          T
        >::type type;
      };/* setProperties */
    };/* struct type */
  };/* make_options */

}/* namespace chimp */

#endif // chimp_make_options_h
