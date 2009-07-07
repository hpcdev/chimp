#include <particledb/interaction/DATACrossSection.h>

#include <olson-tools/xml/physical_parse.h>

namespace particledb {
  namespace interaction {

    using runtime::physical::Quantity;
    using runtime::physical::constant::si::eV;
    using runtime::physical::unit::m;
    using runtime::physical::unit::s;
    using runtime::physical::unit::kg;
    using namespace runtime::physical::math;

    typedef olson_tools::data_set<Quantity,Quantity> pqdata_set;

    DATACrossSection DATACrossSection::load(
                  olson_tools::xml::XMLContext & x,
                  const double & mu ) {
      const Quantity m_s = m/s;
      const Quantity m2  = m*m;
      DATACrossSection cs(mu);
      pqdata_set pqd = x.parse<pqdata_set>();
      /* convert x-val to velocity units. */
      for (pqdata_set::iterator i = pqd.begin(); i != pqd.end(); i++) {
        Quantity v = i->first;
        if (v.units == eV.units)
          v = sqrt(v / (mu*kg) );

        register double v_coeff, m2_coeff;
        v.assertMatch(m_s).getCoeff(v_coeff);
        i->second.assertMatch(m2).getCoeff(m2_coeff);

        cs.table.insert( std::make_pair( v_coeff, m2_coeff) );
      }

      return cs;
    }

  } /* namespace particledb::interaction */
} /* namespace particledb */
