#include "../physical_parse.h"

#include "DATACrossSection.h"

namespace particledb { namespace interaction {

using physical::Quantity;
using physical::constant::eV;
using physical::unit::m;
using physical::unit::s;
using physical::unit::kg;
using namespace physical::math;

typedef data_set<Quantity,Quantity> pqdata_set;

DATACrossSection DATACrossSection::load( xml::XMLContext & x, const double & mu) {
    const Quantity m_s = m/s;
    const Quantity m2  = m*m;
    DATACrossSection cs(mu);
    pqdata_set pqd = x.parse<pqdata_set>();
    /* convert x-val to velocity units. */
    for (pqdata_set::iterator i = pqd.begin(); i != pqd.end(); i++) {
        Quantity v = i->first;
        if (v.units == eV.units)
            v = sqrt(v / (mu*kg) );

        cs.table.insert(
            std::make_pair(
                v.assertMatch(m_s).coeff,
                i->second.assertMatch(m2).coeff
            )
        );
    }

    return cs;
}

}} /* namespace particledb::interaction.*/
