/** \file
 * Declaration of interaction::model::Elastic class.
 */

#ifndef chimp_interaction_model_Elastic_h
#define chimp_interaction_model_Elastic_h

#include <chimp/interaction/Term.h>
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/model/Base.h>
#include <chimp/interaction/ReducedMass.h>
#include <chimp/interaction/ParticleAccessors.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/xml/Doc.h>
#include <olson-tools/random/random.h>

#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <cassert>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an elastic interaction model. */
      template < typename options >
      struct Elastic : Base<options> {
        /* TYPEDEFS */
        typedef typename Base<options>::ParticleParam ParticleParam;
        typedef typename options::Particle Particle;


        /* STATIC STORAGE */
        static const std::string label;


        /* MEMBER STORAGE */
        /** Reduced mass related ratios. */
        ReducedMass mu;


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        Elastic() : mu() { }

        /** Constructor. */
        Elastic( const interaction::Equation<options> & eq )
          : mu( eq.reducedMass ) { }

        /** Constructor that specifies the reduced mass explicitly. */
        Elastic( const ReducedMass & mu )
          : mu( mu ) { }

        /** Virtual NO-OP destructor. */
        virtual ~Elastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Two-body collision interface. */
        virtual void interact( const Particle & part1,
                               const Particle & part2,
                               std::vector< ParticleParam > & products )  {
          products.resize( 2u );
          products[0].is_set = true;
          products[1].is_set = true;

          interact( products[0].particle = part1,
                    products[1].particle = part2 );
        }

        /** Three-body collision interface. */
        virtual void interact( const Particle & part1,
                               const Particle & part2,
                               const Particle & part3,
                               std::vector< ParticleParam > & products ) {
          throw std::runtime_error(
            "Three body interactions are not supported by Elastic collisions"
          );
        }

        /** Binary elastic collision. */
        void interact( Particle & part1, Particle & part2 ) {
          using olson_tools::SQR;
          using olson_tools::fast_pow;
          using olson_tools::Vector;
          using olson_tools::V3;
          using olson_tools::random::MTRNGrand;

          /* This copy allows the Particle class to have other storage instead
           * of just double perhaps. */
          const Vector<double,3> & v1 = velocity(part1);
          const Vector<double,3> & v2 = velocity(part2);

          /*  first obtain the center of mass velocity components */

          /* velocity of center of mass. */
          Vector<double,3> VelCM = (mu.over_m2 * v1) +
                                   (mu.over_m1 * v2);

          /* relative velocity prior to collision */
          Vector<double,3> VelRelPre = v1 - v2;
          double SpeedRel = VelRelPre.abs();

          // use the VHS logic
          double B = 2.0 * MTRNGrand() - 1.0;
          // B is the cosine of a random elevation angle
          double A = std::sqrt( 1.0 - SQR(B) );
          // C is a random azimuth angle
          double C = 2.0 * M_PI * MTRNGrand();

          /* relative velocity after collision */
          Vector<double,3> VelRelPost =
            V3( B * SpeedRel,
                A * std::cos(C) * SpeedRel,
                A * std::sin(C) * SpeedRel );

          // VelRelPost is the post-collision relative v.
          velocity(part1) = VelCM + ( mu.over_m1 * VelRelPost );
          velocity(part2) = VelCM - ( mu.over_m2 * VelRelPost );
        } // collide

        /** load a new instance of the Interaction. */
        virtual Elastic * new_load( const xml::Context & x,
                                    const interaction::Equation<options> & eq,
                                    const RuntimeDB<options> & db ) const {
          return new Elastic( eq.reducedMass );
        }

      };

      template < typename options >
      const std::string Elastic<options>::label = "elastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_Elastic_h
