/** \file
 * Declaration of interaction::model::InElastic class.
 */

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>
#include <olson-tools/random/random.h>

#include <stdexcept>
#include <cmath>

namespace chimp {
  namespace interaction {
    namespace model {

      template < typename T >
      inline void InElastic<T>::interact( Particle & part1, Particle & part2 ) {
        throw std::logic_error("Inelastic interactions not implemented yet!!!")
      } // interact


      virtual InElastic * new_load( const xml::Context & x,
                                    const interaction::Input & input,
                                    const RuntimeDB<options> & db ) const {
        if ( two-body interaction )
          return new InElastic/*( input.A, input.B, db )*/;
        else
          return new InElasticNBody( input, db );
      }

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

