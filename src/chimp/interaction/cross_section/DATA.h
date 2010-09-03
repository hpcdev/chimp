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
 * Emperical data cross section provider class.
 * */

#ifndef chimp_interaction_cross_section_DATA_h
#define chimp_interaction_cross_section_DATA_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/cross_section/detail/logE_E.h>
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/ReducedMass.h>

#include <xylose/power.h>
#include <xylose/logger.h>
#include <xylose/Vector.h>
#include <xylose/xml/Doc.h>
#include <xylose/strutil.h>
#include <xylose/data_set.h>

#include <ostream>
#include <stdexcept>
#include <string>

#include <cstdlib>

namespace chimp {
  namespace xml = xylose::xml;

  namespace interaction {
    namespace cross_section {

      typedef xylose::data_set<double,double> DoubleDataSet;

      /** Load a cross section data set from an appropriate xml::Context after
       * converting the x-axis into velocity and the y axis into cross section,
       * all in SI units.
       *
       * @param x
       *     The context of the cross-section from which to load the
       *     cross-section data set.
       * @param mu
       *     Reduced mass of particles in question.
       * */
      DoubleDataSet loadCrossSectionData( const xml::Context & x,
                                          const ReducedMass & mu );

      /** Emperical data cross section provider.
       * @tparam options
       *    The RuntimeDB template options (see make_options::type for the
       *    default options class).  
       */
      template < typename options >
      class DATA : public cross_section::Base<options> {
        /* STATIC STORAGE */
      public:
        static const std::string label;


        /* MEMBER STORAGE */
      private:
        /** Table of cross-section data. */
        DoubleDataSet table;

        /** Extrapolation coefficient C in C*b*ln(a*(v^2-v0^2+b)/(a*(v^2-v0^2+b). */
        double C;

        /** Extrapolation coefficient a in C*b*ln(a*(v^2-v0^2+b)/(a*(v^2-v0^2+b). */
        double a;

        /** Extrapolation coefficient b in C*b*ln(a*(v^2-v0^2+b)/(a*(v^2-v0^2+b). */
        double b;

        /** Extrapolation coeff.    v^2 in C*b*ln(a*(v^2-v0^2+b)/(a*(v^2-v0^2+b). */
        double v02;

        /** Extrapolation warning issued already. */
        int * extraps_done;


        /* MEMBER FUNCTIONS */
      public:
        /** Default constructor creates a DATA instance with no data.  This
         * is primarily useful for obtaining a class from which to call
         * DATA::new_load. 
         */
        DATA() : cross_section::Base<options>(), a(0.0), b(0.0) {
          extraps_done = new int;
        }

        /** Constructor with the reduced mass already specified. */
        DATA( const xml::Context & x,
              const ReducedMass & mu )
          : table( loadCrossSectionData(x, mu ) ) {
          extraps_done = new int;
          setCoeffs();
        }

        /** Constructor to initialize the cross section data by copying from a
         * set of data previously loaded. */
        DATA( const DoubleDataSet & table )
          : cross_section::Base<options>(), table( table ) {
          extraps_done = new int;
          setCoeffs();
        }

        /** Virtual NO-OP destructor. */
        virtual ~DATA() {
          delete extraps_done;
        }

        /** Interpolate the cross-section from a lookup table.
         *
         * @param v_relative
         *     The relative velocity between two particles.
         * */
        inline virtual double operator() (const double & v_relative) const {
          using xylose::SQR;

          /* find the first entry not less that v_relative */
          DoubleDataSet::const_iterator i = table.lower_bound(v_relative);
          if      (i==table.begin()) {
            /* Assume that the data begins at a threshold value */
            return 0;
          } else if (i==table.end()) {
            --i;
            if ( i->second == 0.0 )
              /* the data actually says to stay at zero--no extrap. required.*/
              return 0.0;

            if (C == 0.0)
              /* We cannot do any extrapolation because a,b are mostly bogus. */
              throw std::runtime_error(
                "velocity " + xylose::to_string(v_relative) +
                " out of range of cross section data" );

            if ( ! *extraps_done ) {
              ++(*extraps_done);
              using xylose::logger::log_warning;
              log_warning( "extrapolating cross section DATA at v=%g",
                           v_relative );
              log_warning( "NOTE:  extrapolation warning only issued once!" );
            }


            using detail::f;
            return f(SQR(v_relative) - v02, C, a, b);
          } else {
            DoubleDataSet::const_iterator f = i;
            --i;
            /* we are not at the ends of the data, so use the normal lever rule.
             * TODO:  Do we need to do the L_inv mult befoe the add to avoid
             * precision errors?  Does our data ever require this? */
            double L_inv = 1.0/(f->first - i->first);
            return   i->second * L_inv * (f->first - v_relative) +
                     f->second * L_inv * (v_relative - i->first);
          }
        }

        /** Determine by inspection the maximum value of the product v_rel *
         * cross_section given a specific maximum v_rel to include in the search. */
        virtual double findMaxSigmaVProduct(const double & v_rel_max) const {
          /* search through the data within the range [0:v_rel_max) to find
           * maximum product. */
          double retval = 0;
          /* find the first entry not less that v_rel_max */
          DoubleDataSet::const_iterator f = table.lower_bound(v_rel_max);
          for (DoubleDataSet::const_iterator i = table.begin(); i != f; ++i) {
            double prod_i = i->first * i->second;
            if (retval < prod_i)
              retval = prod_i;
          }

          return retval;
        }

        virtual DATA * new_load( const xml::Context & x,
                                 const interaction::Equation<options> & eq,
                                 const RuntimeDB<options> & db ) const {
          return new DATA( x, eq.reducedMass );
        }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Print the cross section data table. */
        std::ostream & print(std::ostream & out) const {
          out << table;
          return out;
        }

        /** Set the table explicitly. */
        void setTable( const DoubleDataSet & table ) {
          this->table = table;
          setCoeffs();
        }

        /** Copy operator. */
        DATA & operator= ( const DATA & that ) {
          this->table = that.table;
          this->C     = that.C;
          this->a     = that.a;
          this->b     = that.b;
          this->v02   = that.v02;
          *this->extraps_done = *that.extraps_done;
        }

        /** return the number of extrapolations performed till now. */
        int getNumberExtraps() const {
          return *extraps_done;
        }

      private:
        void setCoeffs() {
          C = a = b = v02 = 0.0;
          *extraps_done = 0;

          const std::string no_str = "no";
          const char * env_allowed
            = std::getenv("CHIMP_CROSS_SECTION_EXTRAPOLATION");
          if ( (env_allowed && env_allowed == no_str) ||
               (! env_allowed &&
                ! options::cross_section_data_extrapolation_allowed ) )
            return;

          if (table.size() == 2u) {
            /* we really shouldn't have data like this, but... */
            // FIXME:  set coeffs for linear extrap?
            return;
          } else if (table.size() < 2u) {
            return;
          }

          /* We'll try for the last three points, if we have that many. */
          DoubleDataSet::iterator d0 = table.end(),
                                  d1 = table.end(),
                                  d2 = table.end();
          --d2;
          --d1; --d1;
          --d0; --d0; --d0;

          if ( d2->second == 0.0 )
            /* extrapolation not needed as data goes to zero. */
            return;

          xylose::Vector<double,4u> Cabv2 = detail::getCab_coeffs( *d0, *d1, *d2 );
          C   = Cabv2[0];
          a   = Cabv2[1];
          b   = Cabv2[2];
          v02 = Cabv2[3];
        }
      };

      template < typename options >
      const std::string DATA<options>::label = "data";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_DATA_h
