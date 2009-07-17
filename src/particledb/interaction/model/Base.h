// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_Base_h
#define particledb_interaction_model_Base_h

#include <particledb/interaction/Input.h>

#include <olson-tools/xml/Doc.h>

namespace particledb {
  template < typename T > class RuntimeDB;

  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace model {

      template < typename options >
      struct Base {
        /* TYPEDEFS */
        typedef typename options::Particle Particle;

        /** Virtual NO-OP destructor. */
        virtual ~Base() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const = 0;

        /** Binary collision interface. */
        virtual void interact( Particle & part1, Particle & part2 ) = 0;

        /** load a new instance of the Interaction. */
        virtual Base * new_load( const xml::Context & x,
                                 const interaction::Input & input,
                                 const RuntimeDB<options> & db ) const = 0;

      };

    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_Base_h
