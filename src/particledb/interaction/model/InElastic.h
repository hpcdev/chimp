// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_InElastic_h
#define particledb_interaction_model_InElastic_h

#include <particledb/RuntimeDB.h>
#include <particledb/interaction/Input.h>
#include <particledb/interaction/model/Base.h>
#include <particledb/interaction/model/detail/ReducedMass.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/indices.h>
#include <olson-tools/xml/Doc.h>
#include <olson-tools/random/random.h>

#include <string>
#include <cmath>

namespace particledb {
  namespace interaction {
    namespace model {

      template < typename options >
      struct InElastic : Base<options> {

        /* STATIC STORAGE */
        static const std::string label;



        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~InElastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Binary inelastic collision. */
        virtual void interact( Particle & part1, Particle & part2 ) {
        } // interact

        /** load a new instance of the Interaction. */
        virtual InElastic * new_load( const xml::Context & x,
                                      const interaction::Input & input,
                                      const RuntimeDB<options> & db ) const {
          return new InElastic/*( input.A, input.B, db )*/;
        }

      };

      template < typename options >
      const std::string InElastic<options>::label = "inelastic";

    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_InElastic_h
