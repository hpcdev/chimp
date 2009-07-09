// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_InElastic_h
#define particledb_interaction_model_InElastic_h

#include <particledb/interaction/Input.h>
#include <particledb/interaction/model/Base.h>
#include <particledb/interaction/model/detail/ReducedMass.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/indices.h>
#include <olson-tools/xml/XMLDoc.h>
#include <olson-tools/random/random.h>

#include <cmath>

namespace particledb {
  namespace interaction {
    namespace model {

      template < typename options >
      struct InElastic : Base<options> {



        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~InElastic() { }

        /** Binary inelastic collision. */
        virtual void interact( Particle & part1, Particle & part2 ) {
        } // interact

        /** load a new instance of the Interaction. */
        template < typename RnDB >
        virtual InElastic * new_load( xml::XMLContext & x,
                                      const interaction::Input & input,
                                      const RnDB & db ) {
          return new InElastic( input.A, input.B, db );
        }

      };

    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_InElastic_h
