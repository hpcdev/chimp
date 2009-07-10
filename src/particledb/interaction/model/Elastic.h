// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_Elastic_h
#define particledb_interaction_model_Elastic_h

#include <particledb/RuntimeDB.h>
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
      struct Elastic : Base<options> {
        /* TYPEDEFS */
        typedef property::mass mass;

        /* MEMBER STORAGE */
        detail::ReducedMass mu;


        /* MEMBER FUNCTIONS */
        /** Default constructor sets mu to invalid values. */
        Elastic() : mu() { }

        /** Constructor. */
        Elastic( const int & type0,
                 const int & type1,
                 const RuntimeDB<options> & db )
          : mu( db[type0].mass::value, db[type1].mass::value ) { }

        /** Virtual NO-OP destructor. */
        virtual ~Elastic() { }

        /** Binary elastic collision. */
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



          // use the VHS logic
          double B = 2.0 * MTRNGrand() - 1.0;
          // B is the cosine of a random elevation angle
          double A = std::sqrt( 1.0 - SQR(B) );
          VelRelPost[X] = B * SpeedRel;
          double C = 2.0 * M_PI * MTRNGrand();
          // C is a random azimuth angle
          VelRelPost[Y] = A * std::cos(C) * SpeedRel;
          VelRelPost[Z] = A * std::sin(C) * SpeedRel;




          // VelRelPost is the post-collision relative v.
          part1.v = VelCM + ( mu.over_m1 * VelRelPost );
          part2.v = VelCM - ( mu.over_m2 * VelRelPost );

          // // or, for similar particles
          // VelRelPost *= 0.5;
          // part1.v = VelCM + ( VelRelPost );
          // part2.v = VelCM - ( VelRelPost );

        } // collide

        /** load a new instance of the Interaction. */
        virtual Elastic * new_load( const xml::XMLContext & x,
                                    const interaction::Input & input,
                                    const RuntimeDB<options> & db ) const {
          return new Elastic( input.A, input.B, db );
        }

      };

    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_Elastic_h
