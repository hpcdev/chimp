#ifndef particledb_interaction_VHSCrossSection_h
#define particledb_interaction_VHSCrossSection_h

#include <particledb/interaction/CrossSection.h>
#include <particledb/interaction/VHSInfo.h>

#include <olson-tools/power.h>
#include <olson-tools/xml/Doc.h>

#include <physical/physical.h>

#include <ostream>

namespace particledb {
  namespace xml = olson_tools::xml;

  namespace interaction {

    struct VHSCrossSection : CrossSection {
      /* MEMBER STORAGE */
      /** The vhs information for this particular interaction. */
      VHSInfo vhs;




      /* MEMBER FUNCTIONS */
      /** Constructor; sets reduced mass to zero by default. */
      VHSCrossSection(const double & mu = 0.0) : CrossSection(mu) {}

      /** Virtual NO-OP destructor. */
      virtual ~VHSCrossSection() {}

      /** Compute the cross section.
       * This implements the variable hard-sphere model as
       * described in equation 4.63 in Graeme Bird's book.
       *
       * @param v_relative
       *     The relative velocity between two particles.
       * */
      virtual inline double cross_section(const double & v_relative) const {
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

      virtual double find_max_sigma_v_rel(const double & v_rel_max) const {
        /* just return the product since the product is monotonically
         * increasing. */
        return cross_section(v_rel_max)*v_rel_max;
      }

      virtual VHSCrossSection * new_load( xml::Context & x,
                                          const double & mu ) const {
        return new VHSCrossSection( load(x,mu) );
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
      static VHSCrossSection load(xml::Context & x, const double & mu) {
        VHSCrossSection cs(mu);
        cs.vhs = VHSInfo::load(x);
        return cs;
      }
    };

  } /* namespace particledb::interaction */
} /* namespace particledb */

#endif // particledb_interaction_VHSCrossSection_h
