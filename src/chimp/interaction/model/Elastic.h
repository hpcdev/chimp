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

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/indices.h>
#include <olson-tools/xml/Doc.h>
#include <olson-tools/random/random.h>

#include <string>
#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {

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
        virtual void interact( Particle & part1, Particle & part2 ) {
          using olson_tools::SQR;
          using olson_tools::fast_pow;
          using olson_tools::Vector;
          using olson_tools::V3;
          using namespace olson_tools::indices;

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
        virtual Elastic * new_load( const xml::Context & x,
                                    const interaction::Input & input,
                                    const RuntimeDB<options> & db ) const {
          return new Elastic( input.A, input.B, db );
        }

      };

      template < typename options >
      const std::string Elastic<options>::label = "elastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_Elastic_h
