#ifndef PARTICLEDB_VHSPROPERTIES_H
#define PARTICLEDB_VHSPROPERTIES_H

#include "Interaction.h"
#include "data_set.h"

#include <olson-tools/physical/physical.h>

#include <map>

namespace particledb { namespace Interaction {

struct DATACrossSection : CrossSection {
    data_set<double,double> table;

    virtual ~DATACrossSection() {}

    /** Interpolate the cross-section from a lookup table.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    virtual double cross_section(const double & v_relative) const {
        return 0;
    }

    /** Load the information into this properties node.
     * */
    static DATACrossSection load(XMLContext & x, const double & mu);
};

}} /* namespace particledb::Interaction.*/

#endif // PARTICLEDB_VHSPROPERTIES_H
