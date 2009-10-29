/** \file
 * Declaration of interaction::model::Elastic class.
 */

#include <chimp/interaction/model/Elastic.h>
#include <chimp/interaction/model/ElasticNBody.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/random/random.h>

#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {

      template < typename T >
      inline void Elastic<T>::interact( Particle & part1, Particle & part2 ) {
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

      template < typename T >
      inline Elastic *
      Elastic<T>::new_load( const xml::Context & x,
                            const interaction::Input & input,
                            const RuntimeDB<options> & db ) const {
        if ( two-body interaction )
          return new Elastic( input.A, input.B, db );
        else
          return new ElasticNBody( input, db );
      }

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

