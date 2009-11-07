/** \file
 * Cross section definition using the variable hard sphere (VHS) model.
 * */

#ifndef chimp_interaction_cross_section_VHS_h
#define chimp_interaction_cross_section_VHS_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/cross_section/detail/VHSInfo.h>
#include <chimp/interaction/ReducedMass.h>

#include <olson-tools/power.h>
#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <ostream>

namespace chimp {
  namespace xml = olson_tools::xml;

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

        /** Constructor that uses the specified inputs and the database to
         * calculate the reduced mass. */
        VHS( const xml::Context & x,
             const interaction::Input & input,
             const RuntimeDB<options> & db )
        : cross_section::Base<options>(),
          vhs( detail::VHSInfo::load(x) ),
          mu( input, db ) { }

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
          using olson_tools::SQR;
          using olson_tools::fast_pow;

          return 
                  /* the collision cross-section is based on
                   * eqn (4.63) for VHS model. */
              vhs.cross_section
            * fast_pow(
                ( 2.0 * K_B * vhs.T_ref
                  / ( mu.value
                      * SQR(v_relative)
                    )
                ), (vhs.visc_T_law - 0.5))
            * vhs.gamma_visc_inv;
        }

        virtual double findMaxSigmaVProduct(const double & v_rel_max) const {
          /* just return the product since the product is monotonically
           * increasing. */
          return operator()(v_rel_max) * v_rel_max;
        }

        virtual VHS * new_load( const xml::Context & x,
                                const interaction::Input & input,
                                const RuntimeDB<options> & db ) const {
          return new VHS( x, input, db );
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
