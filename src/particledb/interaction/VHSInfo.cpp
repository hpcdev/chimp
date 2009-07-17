#include <particledb/interaction/VHSInfo.h>

#include <olson-tools/xml/physical_parse.h>

namespace particledb {
  namespace interaction {

    void parse_item( VHSInfo & out, const xml::Context & x ) {
      using runtime::physical::Quantity;
      using runtime::physical::unit::K;
      using runtime::physical::unit::m;
      x.query<Quantity>("value").assertMatch(m*m).getCoeff(out.cross_section);
      x.query<Quantity>("T_ref").assertMatch(K).getCoeff(out.T_ref);
      x.query<Quantity>("visc_T_law").assertUnitless().getCoeff(out.visc_T_law);
      out.compute_gamma_visc_inv();
    }

    VHSInfo VHSInfo::load(xml::Context & x) {
      return x.parse<VHSInfo>();
    }

  } /* namespace particledb::interaction */
} /* namespace particledb */
