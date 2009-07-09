// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#ifndef particledb_interaction_model_detail_ReducedMass_h
#define particledb_interaction_model_detail_ReducedMass_h

#include <limits>

namespace particledb {
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

      } /* namespace particledb::interaction::model::detail */
    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_model_detail_ReducedMass_h
