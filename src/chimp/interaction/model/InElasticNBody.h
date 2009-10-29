/** \file
 * Declaration of interaction::model::InElasticNBody class.
 */

#ifndef chimp_interaction_model_InElasticNBody_h
#define chimp_interaction_model_InElasticNBody_h

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/xml/Doc.h>
#include <olson-tools/random/random.h>

#include <string>
#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an <b>in</b>elastic interaction model. */
      template < typename options >
      struct InElasticNBody : InElastic<options> {

        /* STATIC STORAGE */
        /** The label for n-body in elastic collisions.  This overrides the
         * value from InElastic. */
        static const std::string label;



        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~InElasticNBody() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Binary inelastic collision. */
        virtual void interact( Particle & part1, Particle & part2 ) {
        } // interact

        /** load a new instance of the Interaction. */
        virtual InElasticNBody * new_load( const xml::Context & x,
                                           const interaction::Input & input,
                                           const RuntimeDB<options> & db ) const {
          return new InElasticNBody/*( input.A, input.B, db )*/;
        }

      };

      template < typename options >
      const std::string InElasticNBody<options>::label = "inelastic.n-body";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_InElasticNBody_h
