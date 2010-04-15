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
 * Information required for variable hard sphere (VHS) cross section
 * calculations.
 *
 * @see cross_section::VHS.
 * */

#ifndef chimp_interaction_cross_section_detail_VHSInfo_h
#define chimp_interaction_cross_section_detail_VHSInfo_h

#include <xylose/xml/Doc.h>

#include <ostream>
#include <cmath>

#if (defined(PGCC) || __sun__ == 1)
inline double tgamma(double x) {
  int sign = 0;
  register double lg = lgamma_r(x,&sign);
  return sign * exp(lg);
}
#endif




namespace chimp {
  namespace interaction {
    namespace xml = xylose::xml;
    namespace cross_section {
      namespace detail {

        struct VHSInfo {
          /* MEMBER STORAGE */
          /** A cross section computed, measured, or whatever at the reference
           * temperature T_ref.
           *
           * @see cross_section(const double &).  */
          double cross_section;

          /** Reference temperature for diameter/cross-section.
           * This is used for the VHS (variable hard sphere model).
           * Please don't forget to give sources :-).
           */
          double T_ref;

          /** Viscosity-temperature power law.
           * @see Graham Bird's <b>Molecular Gas Dynamics and the Direct
           * Simulation of Gas Flows</b>.
           */
          double visc_T_law;

          /** The reciprocal of Gamma function of (5/2 - viscosity-temperature power law).
           */
          double gamma_visc_inv;



          /* MEMBER FUNCTIONS */
          /** Compute the inverse of the gamma viscosity law for this cross section.
           * */
          inline void compute_gamma_visc_inv() {
            gamma_visc_inv  = 1.0/tgamma(5.0/2.0 - visc_T_law);
          }

          std::ostream & print(std::ostream & out) const {
            out << "{cross-section: " << cross_section << ", "
                    "T-ref: " << T_ref << ", "
                    "visc-T-law: " << visc_T_law << ", "
                    "1/gamma(5/2- 'visc-T-law'): " << gamma_visc_inv
                << '}';
            return out;
          }



          /* STATIC FUNCTIONS */
          /** Load the information into this properties node.
           *
           * @see DSMCInfo::load().
           * */
          static VHSInfo load(const xml::Context & x);
        };

      } /* namespace chimp::interaction::cross_section::detail */
    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_detail_VHSInfo_h
