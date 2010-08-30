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
 * Averaged cross section provider class.
 * */

#ifndef chimp_interaction_cross_section_AveragedDiameters_h
#define chimp_interaction_cross_section_AveragedDiameters_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/ReducedMass.h>

#include <xylose/xml/Doc.h>
#include <xylose/power.h>

#include <boost/shared_ptr.hpp>

#include <stdexcept>
#include <ostream>

namespace chimp {
  namespace xml = xylose::xml;
  using boost::shared_ptr;

  namespace interaction {
    namespace cross_section {

      /** Averaged cross section provider.
       * @tparam options
       *    The RuntimeDB template options (see make_options::type for the
       *    default options class).  
       */
      template < typename options >
      struct AveragedDiameters : cross_section::Base<options> {
        /* TYPEDEFS */
        typedef shared_ptr< cross_section::Base<options> > CSPtr;


        /* STATIC STORAGE */
        static const std::string label;


        /* MEMBER STORAGE */
        /** Pointers to the sub-cross-section instances. */
        CSPtr cs0, cs1;


        /* MEMBER FUNCTIONS */
        /** Constructor to initialize the cross section data by copying from a
         * set of data previously loaded. */
        AveragedDiameters( const CSPtr & cs0, const CSPtr & cs1 )
          : cross_section::Base<options>(), cs0(cs0), cs1(cs1) {}

        /** Virtual NO-OP destructor. */
        virtual ~AveragedDiameters() {}

        /** Interpolate the cross-section from a lookup table.
         *
         * @param v_relative
         *     The relative velocity between two particles.
         * */
        inline virtual double operator() (const double & v_relative) const {
          using xylose::SQR;
          return 0.25 * SQR( sqrt(cs0->operator()(v_relative)) +
                             sqrt(cs1->operator()(v_relative)) );
        }

        /** Determine by inspection the maximum value of the product v_rel *
         * cross_section given a specific maximum v_rel to include in the search. */
        virtual std::pair<double,double>
        findMaxSigmaV(const double & v_rel_max) const {
          std::pair<double,double> sv0 = cs0->findMaxSigmaV(v_rel_max);
          std::pair<double,double> sv1 = cs1->findMaxSigmaV(v_rel_max);

          // FIXME: I know that this is hokey, but until I figure out something
          // smarter, I'll just return ...

          double csv0 = sv0.first + sv0.second * cs1->operator()(sv0.second);
          double csv1 = sv1.first + sv1.second * cs0->operator()(sv1.second);
          if ( csv0 > csv1 )
            return std::make_pair( csv0, sv0.second );
          else
            return std::make_pair( csv1, sv1.second );
        }

        virtual AveragedDiameters * new_load( const xml::Context & x,
                                 const interaction::Equation<options> & eq,
                                 const RuntimeDB<options> & db ) const {
          throw std::runtime_error("AveragedDiameters::new_load doesn't work yet");
        }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Print to stream... */
        std::ostream & print(std::ostream & out) const {
          out << "{# Averaged Diameters of :\n";
          cs0.print(out << "1:\n") << '\n';
          cs1.print(out << "2:\n") << '}';
          return out;
        }

      };

      template < typename options >
      const std::string AveragedDiameters<options>::label = "averaged_diameters";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_AveragedDiameters_h
