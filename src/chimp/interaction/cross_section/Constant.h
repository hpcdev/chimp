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
 * Constant cross section provider class.
 * */

#ifndef chimp_interaction_cross_section_Constant_h
#define chimp_interaction_cross_section_Constant_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/Equation.h>

#include <xylose/xml/Doc.h>

#include <ostream>
#include <string>

namespace chimp {
  namespace xml = xylose::xml;

  namespace interaction {
    namespace cross_section {

      namespace detail {
        double loadConstantValue( const xml::Context & x );
      }

      /** Constant cross section provider.
       * @tparam options
       *    The RuntimeDB template options (see make_options::type for the
       *    default options class).  
       */
      template < typename options >
      class Constant : public cross_section::Base<options> {
        /* STATIC STORAGE */
      public:
        static const std::string label;


        /* MEMBER STORAGE */
        /** Extrapolation warning issued already. */
        double value;


        /* MEMBER FUNCTIONS */
      public:
        /** Default constructor creates a Constant instance with no data.  This
         * is primarily useful for obtaining a class from which to call
         * Constant::new_load. 
         */
        Constant()
          : cross_section::Base<options>(), value(0.0) { }

        /** Constructor to load from specific xml context. */
        Constant( const xml::Context & x )
          : value( detail::loadConstantValue(x) ) { }

        /** Constructor to initialize the cross section specifically. */
        Constant( const double & value )
          : cross_section::Base<options>(), value( value ) { }

        /** Virtual NO-OP destructor. */
        virtual ~Constant() { }

        /** Interpolate the cross-section from a lookup table.
         *
         * @param v_relative
         *     The relative velocity between two particles.
         * */
        inline virtual double operator() (const double & v_relative) const {
          return value;
        }

        /** Determine by inspection the maximum value of the product v_rel *
         * cross_section given a specific maximum v_rel to include in the search.
         *
         * @return A pair where
         * .first == ( v * sigma(v) )_max,
         * and
         * .second == v at ( v * sigma(v) )_max
         */
        virtual std::pair<double,double>
        findMaxSigmaV(const double & v_rel_max) const {
          return std::make_pair( value * v_rel_max, v_rel_max);
        }

        virtual Constant * new_load( const xml::Context & x,
                                 const interaction::Equation<options> & eq,
                                 const RuntimeDB<options> & db ) const {
          return new Constant( x );
        }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Print the cross section value. */
        std::ostream & print(std::ostream & out) const {
          out << value;
          return out;
        }
      };

      template < typename options >
      const std::string Constant<options>::label = "constant";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_Constant_h
