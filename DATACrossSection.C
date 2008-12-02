#include "physical_calc.h"

namespace particledb { namespace Interaction {

using physical::constant::eV;
using physical::unit::m;
using physical::unit::s;
using physical::Quantity;

typedef data_set<Quantity,Quantity> pqdata_set;

static DATACrossSection DATACrossSection::load(XMLContext & x, const double & reduced_mass) {
    DATACrossSection cs;

    pqdata_set pqd = x.parse<pqdata_set>();
    pqdata_set pqdo = x.parse<pqdata_set>();
    /* convert x-val to velocity units. */
    for (pqdata_set::iterator i = pqd.begin(); i != pqd.end(); i++)
        if (i->first.units == eV.units)
            pqdo.insert(
                std::make_pair(
                    sqrt(2*i->first / reduced_mass),
                    i->second
                )
            );
    /* now, convert all to doubles, asserting the proper units. */
    std::pair<Quantity,Quantity> u = std::make_pair(m/s, m*m);
    cs.table = convert_data_set<double,double>(pdqo.begin(), pdqo.end(), u);
}

}} /* namespace particledb::Interaction.*/
