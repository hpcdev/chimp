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
 * Cross section definition using the variable hard sphere (VHS) model.
 * */

#ifndef chimp_interaction_cross_section_VHS_h
#define chimp_interaction_cross_section_VHS_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/cross_section/detail/VHSInfo.h>
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/ReducedMass.h>

#include <xylose/power.h>
#include <xylose/xml/Doc.h>

#include <physical/physical.h>

#include <ostream>
#include <limits>

namespace chimp {
  namespace xml = xylose::xml;

  namespace interaction {
    namespace cross_section {

      /** Variable hard sphere implementation of the cross_section::Base class.
       * @tparam options
       *    The RuntimeDB template options (see make_options::type for the
       *    default options class).  
       */
      template < typename options >
      struct VHS : cross_section::Base<options> {
        /* STATIC STORAGE */
        static const std::string label;


        /* MEMBER STORAGE */
        /** The vhs information for this particular interaction. */
        detail::VHSInfo vhs;

        /** The reduced mass. */
        ReducedMass mu;




        /* MEMBER FUNCTIONS */
        /** Default constructor creates a VHS instance with invalid data.  This
         * is primarily useful for obtaining a class from which to call
         * VHS::new_load. 
         */
        VHS() : cross_section::Base<options>() { }

        /** Constructor with the reduced mass already specified. */
        VHS( const xml::Context & x,
             const ReducedMass & mu )
        : cross_section::Base<options>(),
          vhs( detail::VHSInfo::load(x) ),
          mu( mu ) { }

        /** Virtual NO-OP destructor. */
        virtual ~VHS() {}

        /** Compute the cross section.
         * This implements the variable hard-sphere model as
         * described in equation 4.63 in Graeme Bird's book.
         *
         * @param v_relative
         *     The relative velocity between two particles.
         * */
        inline virtual double operator() (const double & v_relative) const {
          using physical::constant::si::K_B;
          using xylose::SQR;
          using xylose::fast_pow;

          /* the collision cross-section is based on eqn (4.63) for VHS model.
           * NOTE that we are guarding against (1/0).
           */
          return 
              vhs.cross_section
            * fast_pow(
                ( 2.0 * K_B * vhs.T_ref
                  / ( mu.value * SQR(v_relative)
                      + std::numeric_limits<double>::min()
                    )
                ), (vhs.visc_T_law - 0.5))
            * vhs.gamma_visc_inv;
        }

        virtual std::pair<double,double>
        findMaxSigmaV(const double & v_rel_max) const {
          /* just return the product since the product is monotonically
           * increasing. */
          return std::make_pair(operator()(v_rel_max) * v_rel_max, v_rel_max);
        }

        virtual VHS * new_load( const xml::Context & x,
                                const interaction::Equation<options> & eq,
                                const RuntimeDB<options> & db ) const {
          return new VHS( x, eq.reducedMass );
        }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Print the VHS data cross section parameters. */
        std::ostream & print(std::ostream & out) const {
          out << "{reduced-mass: " << mu.value << ", "
              << "vhs: ";
          vhs.print(out) << '}';
          return out;
        }
      };

      template < typename options >
      const std::string VHS<options>::label = "vhs";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_VHS_h
