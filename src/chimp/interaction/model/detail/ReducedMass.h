// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef chimp_interaction_model_detail_ReducedMass_h
#define chimp_interaction_model_detail_ReducedMass_h

#include <limits>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace detail {

        struct ReducedMass {
          double over_m1;
          double over_m2;

          ReducedMass()
            : over_m1(std::numeric_limits<double>::infinity()),
              over_m2(std::numeric_limits<double>::infinity()) { }

          ReducedMass( const double & m1, const double & m2 ) {
            over_m1 = 1./( 1. + m1/m2 );
            over_m2 = 1./( 1. + m2/m1 );
          }
        };

      } /* namespace chimp::interaction::model::detail */
    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_detail_ReducedMass_h
