/** \file
 * Declaration of interaction::model::Base class.
 */

#ifndef chimp_interaction_model_Base_h
#define chimp_interaction_model_Base_h

#include <olson-tools/xml/Doc.h>

namespace chimp {

  namespace xml = olson_tools::xml;

  /* forward declaration of RuntimeDB class. */
  template < typename T > class RuntimeDB;

  namespace interaction {

    /* forward declaration of Equation class. */
    template < typename T > class Equation;

    namespace model {

      /** The base class for interaction models.  */
      template < typename options >
      struct Base {
        /* TYPEDEFS */
        typedef typename options::Particle Particle;

        struct ParticleParam {
          Particle particle;
          bool is_set;
        };


        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~Base() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const = 0;

        /** Two-body collision interface. */
        virtual void interact( const Particle & part1,
                               const Particle & part2,
                               std::vector< ParticleParam > & products ) = 0;

        /** Three-body collision interface. */
        virtual void interact( const Particle & part1,
                               const Particle & part2,
                               const Particle & part3,
                               std::vector< ParticleParam > & products ) = 0;

        /** load a new instance of the Interaction. */
        virtual Base * new_load( const xml::Context & x,
                                 const interaction::Equation<options> & eq,
                                 const RuntimeDB<options> & db ) const = 0;

      };

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_Base_h
