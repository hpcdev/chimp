// $Id$
/*
 * Copyright 2002-2007 Spencer Olson
 * dsmc package.
 *
 */

#include <chimp/RuntimeDB.h>

#include <olson-tools/xml/physical_parse.h>

#include <physical/quantity.h>

#include <string>
#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace detail {

        /** load a new instance of the Interaction. */
        double load_vss_param( const xml::Context & x ) {
          using runtime::physical::Quantity;
          return x.query<Quantity>("vss_param_inv")
                  .assertUnitless().getCoeff<double>();
        }

      } /* namespace chimp::interaction::model::detail */
    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

