// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_VSSElastic_h
#define particledb_interaction_model_VSSElastic_h

#include <particledb/interaction/Input.h>
#include <particledb/interaction/model/Base.h>
#include <particledb/interaction/model/detail/ReducedMass.h>
#include <particledb/property/mass.h>

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
      struct VSSElastic : Base<options> {
        /* TYPEDEFS */
        typedef property::mass mass;

        /* MEMBER STORAGE */
        /** Reduced mass related ratios. */
        detail::ReducedMass mu;

        /** Inverse of VSS model parameter. */
        double vss_param_inv;
        


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        VSSElastic() : mu() { }

        /** Constructor. */
        VSSElastic( const int & type0,
                    const int & type1,
                    const double & vss_param_inv,
                    const PropertiesDB & db )
          : mu( db[type0].mass::value, db[type1].mass::value ),
            vss_param_inv(vss_param_inv) { }

        /** Virtual NO-OP destructor. */
        virtual ~VSSElastic() { }

        /** Binary elastic collision of VHS and VSS models. */
        virtual void interact( Particle & part1, Particle & part2 ) {
          using olson_tools::SQR;
          using olson_tools::fast_pow;
          using olson_tools::Vector;
          using namespace olson_tools::indices;
          Vector<double,3> VelCM; /* velocity of center of mass. */
          Vector<double,3> VelRelPre; /* relative velocity prior to collision */
          Vector<double,3> VelRelPost;/* relative velocity after collision */
          double SpeedRel = 0;

          /*  first obtain the center of mass velocity components */

          VelCM = (mu.over_m2 * part1.v)
                + (mu.over_m1 * part2.v);

          // // or for similar particle types:
          // VelCM = part1.v + part2.v; VelCM *= 0.5;

          VelRelPre = part1.v - part2.v;
          SpeedRel = VelRelPre.abs();



          // use the VSS logic
          double B = 2.0 * fast_pow( MTRNGrand(), vss_param_inv ) - 1.0;
          // B is the cosine of the deflection angle for the VSS model (eqn (11.8)
          double A = std::sqrt( 1.0 - B*B);
          double C = 2.0 * M_PI * MTRNGrand();
          double COSC = std::cos(C);
          double SINC = std::sin(C);
          double D = std::sqrt( SQR(VelRelPre[Y]) + SQR(VelRelPre[Z]) );
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
        template < typename RnDB >
        virtual VSSElastic * new_load( xml::XMLContext & x,
                                       const interaction::Input & input,
                                       const RnDB & db ) {
          double vss_param_inv = 
            x.query<Quantity>("vss_param_inv").assertUnitless().getCoeff();
          return new VSSElastic( input.A, input.B, vss_param_inv, db );
        }

      };

    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_VSSElastic_h
