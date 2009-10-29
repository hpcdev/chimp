/** \file
 * Declaration of interaction::model::Elastic class.
 */

#ifndef chimp_interaction_model_Elastic_h
#define chimp_interaction_model_Elastic_h

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/Term.h>
#include <chimp/interaction/Input.h>
#include <chimp/interaction/model/Base.h>
#include <chimp/interaction/model/detail/ReducedMass.h>
#include <chimp/property/mass.h>

#include <olson-tools/xml/Doc.h>

#include <string>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an elastic interaction model for two-body
       * interactions.  For n-body interactions, the Elastic::new_load function
       * will automatically load the ElasticNBody class. 
       *
       * @see ElasticNBody
       */
      template < typename options >
      struct Elastic : Base<options> {
        /* TYPEDEFS */
        typedef property::mass mass;

        /* STATIC STORAGE */
        static const std::string label;

        /* MEMBER STORAGE */
        /** Reduced mass related ratios. */
        detail::ReducedMass mu;


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        Elastic() : mu() { }

        /** Constructor. */
        Elastic( const Term & t0,
                 const Term & t1,
                 const RuntimeDB<options> & db )
          : mu( db[t0.type].mass::value, db[t1.type].mass::value ) { }

        /** Virtual NO-OP destructor. */
        virtual ~Elastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Binary elastic collision. */
        virtual inline void interact( Particle & part1, Particle & part2 );

        /** load a new instance of the Interaction. This function will detect
         * whether the interaction is n-body and will return the ElasticNBody
         * class instead if that is the case.  */
        virtual inline
        Elastic * new_load( const xml::Context & x,
                            const interaction::Input & input,
                            const RuntimeDB<options> & db ) const;

      };

      template < typename options >
      const std::string Elastic<options>::label = "elastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#include <chimp/interaction/model/Elastic.cpp>

#endif // chimp_interaction_model_Elastic_h
