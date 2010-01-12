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


/** \file
 * Details of implementation of electron-iompact ionization model published by
 * Wolfgang Lotz. 
 *
 * Wolfgang Lotz, Z. Physik 216, 241 (1968);
 * Wolfgang Lotz, Z. Physik 220, 466--472 (1969).
 * */

#ifndef chimp_interaction_cross_section_detail_LotzDetails_h
#define chimp_interaction_cross_section_detail_LotzDetails_h

#include <olson-tools/xml/Doc.h>
#include <olson-tools/power.h>

#include <ostream>
#include <cmath>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace cross_section {
      namespace detail {

        struct LotzParameters {
          /* MEMBER STORAGE */
          /** The binding energy of electrons in the ith subshell.
           * \f$ P_1 \f$ is the ionization potential. */
          double P;
          /** Cached result of \f$ \frac{ m_{\rm e} }{ 2 P } \f$ where
           * \f$ m_{\rm e} \f$ is the mass of the electron. */
          double beta;
          /** The number of equivalent electrons in the ith subshell. */
          double q;
          /** A constant determined by ``reasonable guesswork'' as indicated by
           * W. Lotz.  The units of this constant must be
           * \f$ \left[ {\rm length}^2 {\rm energy}^2 \right] \f$.
           */
          double a;
          /** A constant determined by ``reasonable guesswork'' as indicated by
           * W. Lotz.  This constant is unitless. */
          double b;
          /** A constant determined by ``reasonable guesswork'' as indicated by
           * W. Lotz.  This constant is unitless. */
          double c;


          /* MEMBER FUNCTIONS */
          LotzParameters() : P(0), beta(0), q(0), a(0), b(0), c(0) { }
        };

        static inline std::ostream & operator<< ( std::ostream & out,
                                                  const LotzParameters & p ) {
          out << "<Lotz>\n"
                 "  <P>" << p.P << "</P>\n"
                 "  <q>" << p.q << "</q>\n"
                 "  <a>" << p.a << "</a>\n"
                 "  <b>" << p.b << "</b>\n"
                 "  <c>" << p.c << "</c>\n"
                 "</Lotz>";
          return out;
        }

        template < typename LotzT >
        struct SigmaFunctor {
          const LotzT & lotz;
          SigmaFunctor( const LotzT & lotz ) : lotz(lotz) { }
          double operator()( const double & v ) const { return lotz(v); }
        };

        /** The derivative D[v*sigma[v], v]. */
        // FIXME:  I think this is not necessarily correct because of the
        // canceling that I did. 
        template < typename LotzT >
        struct DSigmaVFunctor {
          const LotzT & lotz;
          DSigmaVFunctor( const LotzT & lotz ) : lotz(lotz) { }
          double operator()( const double & v ) const {
            const double v2 = v * v;
            typedef typename LotzT::ParametersVector::const_iterator CIter;
            CIter end = lotz.parameters.end();
            double dsv = 0.0;
            for ( CIter i = lotz.parameters.begin(); i != end; ++i ) {
              using std::log;
              using std::exp;
              using olson_tools::SQR;
              typename LotzT::Parameters const & p = *i;
              const double v2Beta = v2 * p.beta;
              double ln_v2Beta = log( v2Beta );
              double cv2Beta = p.c * v2Beta;
              dsv += p.a * p.q
                   * exp( p.c - cv2Beta ) / ( SQR(p.P) * v2Beta )
                   * (  2. * p.b * ( ln_v2Beta *(0.5 + cv2Beta) - 1.0 )
                      - exp( -p.c + cv2Beta ) * ( ln_v2Beta - 2. ) );
            }

            return dsv;
          }
        };

        void parse_item( LotzParameters & out, const xml::Context & x );

      }/* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_detail_LotzDetails_h
