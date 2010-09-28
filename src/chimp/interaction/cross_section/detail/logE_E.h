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

#include <xylose/power.h>
#include <xylose/Vector.h>
#include <xylose/compat/math.hpp>

namespace chimp {
  namespace interaction {
    namespace cross_section {
      namespace detail {

        /** Calculate the log(E)/E exponential fall-off of the cross section. */
        inline double g(const double & x) {
          register double s = x + M_E;
          return M_E * std::log(s)/s;
        }

        inline double f( const double & x,
                         const double & C,
                         const double & a,
                         const double & b ) {
          register double s = a*x + b;
          return C * b * std::log(s)/s;
        }

        using xylose::Vector;
        /** For fitting a + b*f(x). */
        template < typename Pair >
        Vector<double,4u> getCab_coeffs( const Pair & d0,
                                         const Pair & d1,
                                         const Pair & d2 ) {
          using std::log;
          using xylose::SQR;
          const xylose::make_vector<double,4u> V4
            = xylose::make_vector<double,4u>();

          double Fp = 0.5 * ( (    d1.second -     d0.second) /
                              (SQR(d1.first) - SQR(d0.first)) +
                              (    d2.second -     d1.second) /
                              (SQR(d2.first) - SQR(d1.first)) );
          if ( Fp >= 0.0 )
            return V4(0.0,0.0,0.0,0.0);

          double b = 30.0; // initial guess
          double C = d1.second / log(b);
          double a = b * Fp / ( C * (1. - log(b)) );

          /* now lets refine the guess of b without changing C or a */
          double err_lowest = std::numeric_limits<double>::max();
          for ( double bi = M_E; bi <= 100.0; bi += 0.01 ) {
            double err = 1.0 - d2.second /
                               f(SQR(d2.first) - SQR(d1.first), C, a, bi);
            if ( std::abs(err) < abs(err_lowest) ) {
              b = bi;
              err_lowest = err;
            }
          }

          return V4(C,a,b,SQR(d1.first));
        }

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_detail_logE_E_h
