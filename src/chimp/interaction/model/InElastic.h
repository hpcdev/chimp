/** \file
 * Declaration of interaction::model::InElastic class.
 */

#ifndef chimp_interaction_model_InElastic_h
#define chimp_interaction_model_InElastic_h

#include <chimp/interaction/Equation.h>
#include <chimp/interaction/model/Base.h>

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
      struct InElastic : Base<options> {
        /* TYPEDEFS */
        typedef typename Base<options>::ParticleParam ParticleParam;
        typedef typename options::Particle Particle;


        /* STATIC STORAGE */
        static const std::string label;



        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~InElastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Arbitrary collision participants interface. */
        virtual void interact( const std::vector< const Particle* > & reactants,
                               std::vector< ParticleParam > & products ) {
        }

        /** Binary inelastic collision. */
        virtual void interact( Particle & part1, Particle & part2 ) {
        } // interact

        /** load a new instance of the Interaction. */
        virtual InElastic * new_load( const xml::Context & x,
                                      const interaction::Equation<options> & eq,
                                      const RuntimeDB<options> & db ) const {
          return new InElastic/*( eq.reducedMass )*/;
        }

      };

      template < typename options >
      const std::string InElastic<options>::label = "inelastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_InElastic_h
