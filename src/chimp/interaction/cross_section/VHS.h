/** \file
 * Cross section definition using the variable hard sphere (VHS) model.
 * */

#ifndef chimp_interaction_cross_section_VHS_h
#define chimp_interaction_cross_section_VHS_h

#include <chimp/interaction/cross_section/Base.h>
#include <chimp/interaction/cross_section/detail/VHSInfo.h>

#include <olson-tools/power.h>
#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <ostream>

namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {
    namespace cross_section {

      /** Variable hard sphere implementation of the cross_section::Base class. */
      struct VHS : cross_section::Base {
        /* STATIC STORAGE */
        static const std::string label;


        /* MEMBER STORAGE */
        /** The vhs information for this particular interaction. */
        detail::VHSInfo vhs;




        /* MEMBER FUNCTIONS */
        /** Constructor; sets reduced mass to zero by default. */
        VHS(const double & mu = 0.0) : cross_section::Base(mu) {}

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
                  / ( reduced_mass
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

        virtual VHS * new_load( xml::Context & x,
                                const double & mu ) const {
          return new VHS( load(x,mu) );
        }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** Print the VHS data cross section parameters. */
        std::ostream & print(std::ostream & out) const {
          out << "{reduced-mass: " << reduced_mass << ", "
              << "vhs: ";
          vhs.print(out) << '}';
          return out;
        }

        /** Load the information into this properties node.
         * */
        static VHS load(xml::Context & x, const double & mu) {
          VHS cs(mu);
          cs.vhs = detail::VHSInfo::load(x);
          return cs;
        }
      };

      template < typename options >
      const std::string VHS<options>::label = "vhs";

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_cross_section_VHS_h
