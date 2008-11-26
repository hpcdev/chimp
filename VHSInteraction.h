#ifndef PARTICLEDB_VHSPROPERTIES_H
#define PARTICLEDB_VHSPROPERTIES_H

#include "Interaction.h"

#include <olson-tools/physical.h>

namespace particledb { namespace Interaction {

struct VHSInteraction : Info {
    /** Reduced mass for the input particles. */
    double reduced_mass;

    /** The vhs information for this particular interaction. */
    VHSInfo vhs;

    /** Compute the cross section.
     * This implements the variable hard-sphere model as
     * described in equation 4.63 in Graeme Bird's book.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    inline double vhs_cross_section(const double & v_relative) const {
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

    /** Compute the cross section.
     * This implements the variable hard-sphere model as
     * described in equation 4.63 in Graeme Bird's book.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    virtual double cross_section(const double & v_relative) const {
        return vhs_cross_section(v_relative);
    }

    /** Load the information into this properties node.
     *
     * @see DSMCInfo::load().
     * */
    static VHSInfo load(XMLContext & x);
};

} /* particledb namespace .*/

#endif // PARTICLEDB_VHSPROPERTIES_H
