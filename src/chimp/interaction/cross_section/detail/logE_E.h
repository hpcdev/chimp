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



#ifndef chimp_interaction_cross_section_detail_logE_E_h
#define chimp_interaction_cross_section_detail_logE_E_h

#include <cmath>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace cross_section {
      namespace detail {

        /** Calculate the log(E)/E exponential fall-off of the cross section. */
        inline double g(const double & x) {
          register double s = x + M_E;
          return M_E * std::log(s)/s;
        }

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_detail_logE_E_h
