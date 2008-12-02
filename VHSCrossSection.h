#ifndef PARTICLEDB_VHSPROPERTIES_H
#define PARTICLEDB_VHSPROPERTIES_H

#include "Interaction.h"
#include "VHSInfo.h"

#include <olson-tools/physical/physical.h>

namespace particledb { namespace Interaction {

struct VHSCrossSection : CrossSection {
    /** Reduced mass for the input particles. */
    double reduced_mass;

    /** The vhs information for this particular interaction. */
    VHSInfo vhs;

    virtual ~VHSCrossSection() {}

    /** Compute the cross section.
     * This implements the variable hard-sphere model as
     * described in equation 4.63 in Graeme Bird's book.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    virtual double cross_section(const double & v_relative) const {
        using physical::constant::K_B;
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

    /** Load the information into this properties node.
     * */
    static VHSCrossSection load(XMLContext & x, const double & mu) {
        reduced_mass = mu;
        vhs = VHSInfo::load(x);
    }
};

}} /* namespace particledb::Interaction.*/

#endif // PARTICLEDB_VHSPROPERTIES_H
