/** \file
 * Declaration of interaction::model::VSSElastic class.
 */

#ifndef chimp_interaction_model_VSSElastic_h
#define chimp_interaction_model_VSSElastic_h

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

      namespace detail {
        /** load a new instance of the Interaction. */
        double load_vss_param( const xml::Context & x );
      } /* namespace chimp::interaction::model::detail */


      /** Implementation of a variable-soft-sphere elastic interaction model. */
      template < typename options >
      struct VSSElastic : Base<options> {
        /* TYPEDEFS */
        typedef property::mass mass;

        /* STATIC STORAGE */
        static const std::string label;

        /* MEMBER STORAGE */
        /** Reduced mass related ratios. */
        detail::ReducedMass mu;

        /** Inverse of VSS model parameter. */
        double vss_param_inv;
        


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        VSSElastic() : mu() { }

        /** Constructor. */
        VSSElastic( const Term & t0,
                    const Term & t1,
                    const double & vss_param_inv,
                    const RuntimeDB<options> & db )
          : mu( db[t0.type].mass::value, db[t1.type].mass::value ),
            vss_param_inv(vss_param_inv) { }

        /** Virtual NO-OP destructor. */
        virtual ~VSSElastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Binary elastic collision of VHS and VSS models. */
        virtual void interact( Particle & part1, Particle & part2 ) {
          using olson_tools::SQR;
          using olson_tools::fast_pow;
          using olson_tools::Vector;
          using namespace olson_tools::indices;

          /*  first obtain the center of mass velocity components */

          /* velocity of center of mass. */
          Vector<double,3> VelCM = (mu.over_m2 * part1.v) +
                                   (mu.over_m1 * part2.v);

          /* relative velocity prior to collision */
          Vector<double,3> VelRelPre = part1.v - part2.v;
          double SpeedRel = VelRelPre.abs();

          // use the VSS logic
          double B = 2.0 * fast_pow( MTRNGrand(), vss_param_inv ) - 1.0;
          // B is the cosine of the deflection angle for the VSS model (eqn (11.8)
          double A = std::sqrt( 1.0 - B*B);
          double C = 2.0 * M_PI * MTRNGrand();
          double COSC = std::cos(C);
          double SINC = std::sin(C);
          double D = std::sqrt( SQR(VelRelPre[Y]) + SQR(VelRelPre[Z]) );
          Vector<double,3> VelRelPost;
          if ( D > 1.0E-6 ) {
              VelRelPost[X] = B * VelRelPre[X] + A * SINC * D;
              VelRelPost[Y] = B * VelRelPre[Y] + A * (SpeedRel * VelRelPre[Z] * COSC - VelRelPre[X] * VelRelPre[Y] * SINC)/D;
              VelRelPost[Z] = B * VelRelPre[Z] - A * (SpeedRel * VelRelPre[Y] * COSC + VelRelPre[X] * VelRelPre[Z] * SINC)/D;
          } else {
              VelRelPost[X] = B * VelRelPre[X];
              VelRelPost[Y] = A * COSC * VelRelPre[X];
              VelRelPost[Z] = A * SINC * VelRelPre[X];
          }
          // the post-collision rel. velocity components are based on eqn (2.22)




          // VelRelPost is the post-collision relative v.
          part1.v = VelCM + ( mu.over_m1 * VelRelPost );
          part2.v = VelCM - ( mu.over_m2 * VelRelPost );

          // // or, for similar particles
          // VelRelPost *= 0.5;
          // part1.v = VelCM + ( VelRelPost );
          // part2.v = VelCM - ( VelRelPost );

        } // collide

        /** load a new instance of the Interaction. */
        virtual VSSElastic * new_load( const xml::Context & x,
                                       const interaction::Input & input,
                                       const RuntimeDB<options> & db ) const {
          double vss_param_inv = detail::load_vss_param( x );
          return new VSSElastic( input.A, input.B, vss_param_inv, db );
        }
      };

      template < typename options >
      const std::string VSSElastic<options>::label = "vss_elastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_VSSElastic_h
