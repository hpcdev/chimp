/** \file
 * Declaration of interaction::model::InElastic class.
 */

#ifndef chimp_interaction_model_InElastic_h
#define chimp_interaction_model_InElastic_h

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/Input.h>
#include <chimp/interaction/model/Base.h>
#include <chimp/interaction/model/detail/ReducedMass.h>

#include <olson-tools/xml/Doc.h>

#include <string>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an <b>in</b>elastic interaction model. */
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
        virtual inline void interact( Particle & part1, Particle & part2 );

        /** load a new instance of the Interaction. */
        virtual inline InElastic * new_load( const xml::Context & x,
                                             const interaction::Input & input,
                                             const RuntimeDB<options> & db ) const;

      };

      template < typename options >
      const std::string InElastic<options>::label = "inelastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#include <chimp/interaction/model/InElastic.cpp>

#endif // chimp_interaction_model_InElastic_h
