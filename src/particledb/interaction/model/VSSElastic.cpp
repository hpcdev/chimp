// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#include <particledb/RuntimeDB.h>

#include <olson-tools/xml/physical_parse.h>

#include <physical/quantity.h>

#include <string>
#include <cmath>

namespace particledb {
  namespace interaction {
    namespace model {
      namespace detail {

        /** load a new instance of the Interaction. */
        double load_vss_param( const xml::XMLContext & x ) {
          using runtime::physical::Quantity;
          return x.query<Quantity>("vss_param_inv")
                  .assertUnitless().getCoeff<double>();
        }

      } /* namespace particledb::interaction::model::detail */
    } /* namespace particledb::interaction::model */
  } /* namespace particledb::interaction */
} /* namespace particledb */

