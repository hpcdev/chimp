/** \file
 * Emperical data cross section provider class.
 * */

#ifndef chimp_interaction_cross_section_DATA_h
#define chimp_interaction_cross_section_DATA_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/cross_section/detail/logE_E.h>
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/ReducedMass.h>

#include <olson-tools/xml/Doc.h>
#include <olson-tools/data_set.h>
#include <olson-tools/power.h>

#include <map>
#include <stdexcept>
#include <ostream>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace cross_section {

      typedef olson_tools::data_set<double,double> DoubleDataSet;

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

      /** Scaling constant used for extrapolating data using a ln(E)/E scaling.
       * \todo get the scaling right.
       * for right now, this is an arbitrary scaling.
       */
      const double lne_e_scaling = olson_tools::SQR(2500.0/3e8);

      /** Emperical data cross section provider.
       * @tparam options
       *    The RuntimeDB template options (see make_options::type for the
       *    default options class).  
       */
      template < typename options >
      struct DATA : cross_section::Base<options> {
        /* STATIC STORAGE */
        static const std::string label;


        /* MEMBER STORAGE */
        /** Table of cross-section data. */
        DoubleDataSet table;



        /* MEMBER FUNCTIONS */
        /** Default constructor creates a DATA instance with no data.  This
         * is primarily useful for obtaining a class from which to call
         * DATA::new_load. 
         */
        DATA() : cross_section::Base<options>() {}

        /** Constructor with the reduced mass already specified. */
        DATA( const xml::Context & x,
              const ReducedMass & mu )
          : table( loadCrossSectionData(x, mu ) ) { }

        /** Constructor to initialize the cross section data by copying from a
         * set of data previously loaded. */
        DATA( const DoubleDataSet & table )
          : cross_section::Base<options>(), table( table ) {}

        /** Virtual NO-OP destructor. */
        virtual ~DATA() {}

        /** Interpolate the cross-section from a lookup table.
         *
         * @param v_relative
         *     The relative velocity between two particles.
         * */
        inline virtual double operator() (const double & v_relative) const {
          using olson_tools::SQR;

          /* find the first entry not less that v_relative */
          DoubleDataSet::const_iterator i = table.lower_bound(v_relative);
          if      (i==table.begin()) {
            /* Assume that the data begins at a threshold value */
            return 0;
          } else if (i==table.end()) {
            //throw std::runtime_error("fix extrapolation code please");
            --i;

            using detail::g;
            return i->second * g(SQR(v_relative - i->first)*lne_e_scaling);
          } else {
            DoubleDataSet::const_iterator f = i;
            --i;
            /* we are not at the ends of the data, so use the normal lever
             * rule.  
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

      };

      template < typename options >
      const std::string DATA<options>::label = "data";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_DATA_h
