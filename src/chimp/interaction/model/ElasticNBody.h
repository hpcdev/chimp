/** \file
 * Declaration of interaction::model::ElasticNBody class.
 */

#ifndef chimp_interaction_model_ElasticNBody_h
#define chimp_interaction_model_ElasticNBody_h

#include <chimp/interaction/model/Elastic.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/xml/Doc.h>
#include <olson-tools/random/random.h>

#include <string>
#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an elastic interaction model. */
      template < typename options >
      struct ElasticNBody : Elastic<options> {
        /* TYPEDEFS */
        typedef Elastic<options> super;

        /* STATIC STORAGE */
        /** The label for n-body Elastic collisions.  This overrides the value
         * in Elastic. */
        static const std::string label;


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        ElasticNBody() : super() { }

        /** Constructor. */
        ElasticNBody( const Term & t0,
                      const Term & t1,
                      const RuntimeDB<options> & db )
          : super( t0, t1, db ) { }

        /** Virtual NO-OP destructor. */
        virtual ~ElasticNBody() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Binary elastic collision. */
        virtual void interact( Particle & part1, Particle & part2 ) {
          using olson_tools::SQR;
          using olson_tools::fast_pow;
          using olson_tools::Vector;
          using olson_tools::V3;
          using olson_tools::random::MTRNGrand;

          /*  first obtain the center of mass velocity components */

          /* velocity of center of mass. */
          Vector<double,3> VelCM = (mu.over_m2 * part1.v) +
                                   (mu.over_m1 * part2.v);

          /* relative velocity prior to collision */
          Vector<double,3> VelRelPre = part1.v - part2.v;
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
          part1.v = VelCM + ( mu.over_m1 * VelRelPost );
          part2.v = VelCM - ( mu.over_m2 * VelRelPost );
        } // collide

        /** load a new instance of the Interaction. */
        virtual ElasticNBody * new_load( const xml::Context & x,
                                    const interaction::Input & input,
                                    const RuntimeDB<options> & db ) const {
          return new ElasticNBody( input.A, input.B, db );
        }

      };

      template < typename options >
      const std::string ElasticNBody<options>::label = "elastic.n-body";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_ElasticNBody_h
