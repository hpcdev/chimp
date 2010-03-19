/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Copyright (C) 2006, 2008 Spencer E. Olson                                   *
 *                                                                             *
 * This file is part of CHIMP                                                  *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/



#ifndef chimp_interaction_model_test_diagnostics_h
#define chimp_interaction_model_test_diagnostics_h

#include <chimp/property/mass.h>
#include <chimp/accessors.h>

#include <olson-tools/power.h>
#include <olson-tools/Vector.h>

namespace chimp {
  namespace interaction {
    namespace model {
      namespace test {
        using olson_tools::Vector;

        /** Calculate the energy of a particle. */
        template < typename Particle, typename RnDB  >
        inline double energy( const Particle & p,
                              const int & part_i,
                              const RnDB & db ) {
          using olson_tools::SQR;
          using chimp::accessors::particle::velocity;
          using chimp::accessors::particle::weight;
          const typename RnDB::options::Properties & prop = db[part_i];
          const double & m = prop.property::mass::value;
          double sz = weight(p);
          return sz * 0.5 * m * SQR( velocity(p) );
        }


        /** Calculate the energy of a particle. */
        template < typename Particle, typename RnDB  >
        inline Vector<double,3> momentum( const Particle & p,
                                          const int & part_i,
                                          const RnDB & db ) {
          using chimp::accessors::particle::velocity;
          using chimp::accessors::particle::weight;
          const typename RnDB::options::Properties & prop = db[part_i];
          const double & m = prop.property::mass::value;
          double sz = weight(p);
          return (sz * m) * velocity(p);
        }

      }/* namespace chimp::interaction::model::test */
    }/* namespace chimp::interaction::model */
  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_model_test_diagnostics_h
