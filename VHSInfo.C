#include "VHSProperties.h"
#include "physical_calc.h"

template <> struct parser<VHSInfo>;

VHSInfo VHSInfo::load(XMLContext & x) {
    return x.query<VHSInfo>("VHSData");
}



template <>
struct parser<VHSInfo> {
    static VHSInfo parse(const XMLContext & x) {
        using physical::Quantity;
        using physical::unit::K;
        using physical::unit::m;
        VHSInfo v;
        v.cross_section   = x.query<Quantity>("cross_section" ).assertMatch(m*m).coeff;
        v.T_ref           = x.query<Quantity>("T_ref"         ).assertMatch(K).coeff;
        v.visc_T_law      = x.query<Quantity>("visc_T_law"    ).assertUnitless().coeff;
        v.vss_param_inv   = x.query<Quantity>("vss_param_inv" , 1.0).assertUnitless().coeff;
        v.compute_gamma_visc_inv();
        return v;
    }
};

