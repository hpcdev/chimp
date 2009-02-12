#include <olson-tools/xml/physical_parse.h>

#include "VHSInfo.h"

namespace particledb { namespace interaction {
    void parse_item(VHSInfo & out, const xml::XMLContext & x) {
        using runtime::physical::Quantity;
        using runtime::physical::unit::K;
        using runtime::physical::unit::m;
        out.cross_section   = x.query<Quantity>("value"         ).assertMatch(m*m).coeff;
        out.T_ref           = x.query<Quantity>("T_ref"         ).assertMatch(K).coeff;
        out.visc_T_law      = x.query<Quantity>("visc_T_law"    ).assertUnitless().coeff;
        out.vss_param_inv   = x.query<Quantity>("vss_param_inv" , 1.0).assertUnitless().coeff;
        out.compute_gamma_visc_inv();
    }

    VHSInfo VHSInfo::load(xml::XMLContext & x) {
        return x.parse<VHSInfo>();
    }
}} /* namespace particledb::interaction */
