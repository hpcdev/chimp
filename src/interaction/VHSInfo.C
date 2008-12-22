#include "../physical_parse.h"

#include "VHSInfo.h"

using particledb::interaction::VHSInfo;

namespace particledb {

namespace xml{
    template <> struct parser<VHSInfo>;
}

namespace interaction {
    VHSInfo VHSInfo::load(xml::XMLContext & x) {
        return x.parse<VHSInfo>();
    }
}


namespace xml{
    template <>
    struct parser<VHSInfo> {
        static VHSInfo parse(const XMLContext & x) {
            using physical::Quantity;
            using physical::unit::K;
            using physical::unit::m;
            VHSInfo v;
            v.cross_section   = x.query<Quantity>("value"         ).assertMatch(m*m).coeff;
            v.T_ref           = x.query<Quantity>("T_ref"         ).assertMatch(K).coeff;
            v.visc_T_law      = x.query<Quantity>("visc_T_law"    ).assertUnitless().coeff;
            v.vss_param_inv   = x.query<Quantity>("vss_param_inv" , 1.0).assertUnitless().coeff;
            v.compute_gamma_visc_inv();
            return v;
        }
    };
}

} /* particledb namespace .*/
