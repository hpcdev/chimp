#ifndef PARTICLEDB_VHSPROPERTIES_H
#define PARTICLEDB_VHSPROPERTIES_H

#include "DSMCProperties.h"

#include <olson-tools/physical.h>

#include <math.h>

#if (defined(PGCC) || __sun__ == 1)
inline double tgamma(double x) {
    int sign = 0;
    register double lg = lgamma_r(x,&sign);
    return sign * exp(lg);
}
#endif




namespace particledb {

struct VHSInfo {
    /** A cross section computed, measured, or whatever at the reference
     * temperature T_ref.
     *
     * @see cross_section(const double &).  */
    double cross_section;

    /** Reference temperature for diameter/cross-section.
     * This is used for the VHS (variable hard sphere model).
     * Please don't forget to give sources :-).
     */
    double T_ref;

    /** Viscosity-temperature power law.
     * @see Graham Bird's \underline{Molecular Gas Dynamics and the Direct
     * Simulation of Gas Flows.}
     */
    double visc_T_law;

    /** The reciprocal of Gamma function of (5/2 - viscosity-temperature power law).
     */
    double gamma_visc_inv;

    inline void compute_gamma_visc_inv() {
        gamma_visc_inv  = 1.0/tgamma(5.0/2.0 - visc_T_law);
    }

    /** The reciprocal of the VSS parameter. */
    double vss_param_inv;

    /** Load the information into this properties node.
     *
     * @see DSMCInfo::load().
     * */
    static VHSInfo load(XMLContext & x);
};


} /* particledb namespace .*/

#endif // PARTICLEDB_VHSPROPERTIES_H
