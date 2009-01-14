#ifndef PARTICLEDB_INTERACTION_VHSCROSSSECTION_H
#define PARTICLEDB_INTERACTION_VHSCROSSSECTION_H

#include "CrossSection.h"
#include "VHSInfo.h"

#include <physical/physical.h>
#include <olson-tools/power.h>

#include <ostream>

namespace particledb { namespace interaction {

struct VHSCrossSection : CrossSection {
    /** The vhs information for this particular interaction. */
    VHSInfo vhs;

    VHSCrossSection(const double & mu = 0.0) : CrossSection(mu) {}
    virtual ~VHSCrossSection() {}

    /** Compute the cross section.
     * This implements the variable hard-sphere model as
     * described in equation 4.63 in Graeme Bird's book.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    virtual inline double cross_section(const double & v_relative) const {
        using physical::constant::K_B;
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

    std::ostream & print(std::ostream & out) const {
        out << "{reduced-mass: " << reduced_mass << ", "
            << "vhs: ";
        vhs.print(out) << '}';
        return out;
    }

    /** Load the information into this properties node.
     * */
    static VHSCrossSection load(xml::XMLContext & x, const double & mu) {
        VHSCrossSection cs(mu);
        cs.vhs = VHSInfo::load(x);
        return cs;
    }
};

}} /* namespace particledb::interaction.*/

#endif // PARTICLEDB_INTERACTION_VHSCROSSSECTION_H
