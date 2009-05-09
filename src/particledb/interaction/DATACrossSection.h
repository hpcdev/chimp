#ifndef PARTICLEDB_INTERACTION_DATACROSSSECTION_H
#define PARTICLEDB_INTERACTION_DATACROSSSECTION_H

#include <particledb/interaction/CrossSection.h>

#include <olson-tools/xml/XMLDoc.h>
#include <olson-tools/data_set.h>
#include <olson-tools/power.h>

#include <map>
#include <stdexcept>
#include <ostream>

namespace particledb { namespace interaction {

    inline double g(const double & x) {
        register double s = x + M_E;
        return M_E * log(s)/s;
    }

    /** FIXME:  get the scaling right.
     * for right now, this is an arbitrary scaling. */
    const double lne_e_scaling = olson_tools::SQR(2500.0/3e8);

struct DATACrossSection : CrossSection {
    typedef olson_tools::data_set<double,double> table_t;
    table_t table;

    DATACrossSection(const double & mu = 0.0) : CrossSection(mu) {}
    virtual ~DATACrossSection() {}

    /** Interpolate the cross-section from a lookup table.
     *
     * @param v_relative
     *     The relative velocity between two particles.
     * */
    virtual double cross_section(const double & v_relative) const {
        using olson_tools::SQR;

        /* find the first entry not less that v_relative */
        table_t::const_iterator i = table.lower_bound(v_relative);
        if      (i==table.begin()) {
            /* Assume that the data begins at a threshold value */
            return 0;
        } else if (i==table.end()) {
            //throw std::runtime_error("fix extrapolation code please");
            i--;
            return i->second * g(SQR(v_relative - i->first)*lne_e_scaling);
        } else {
            table_t::const_iterator f = i--;
            /* we are not at the ends of the data, so use the normal lever
             * rule.  
             * TODO:  Do we need to do the L_inv mult befoe the add to avoid
             * precision errors?  Does our data ever require this? */
            double L_inv = 1.0/(f->first - i->first);
            return   i->second * L_inv * (f->first - v_relative)
                   + f->second * L_inv * (v_relative - i->first);
        }
    }

    virtual double find_max_sigma_v_rel(const double & v_rel_max) const {
        /* search through the data within the range [0:v_rel_max) to find
         * maximum product. */
        double retval = 0;
        /* find the first entry not less that v_rel_max */
        table_t::const_iterator f = table.lower_bound(v_rel_max);
        for (table_t::const_iterator i = table.begin(); i != f; i++) {
            double prod_i = i->first * i->second;
            if (retval < prod_i)
                retval = prod_i;
        }

        return retval;
    }

    std::ostream & print(std::ostream & out) const {
        out << table;
        return out;
    }

    /** Load the information into this properties node.
     * @param x
     *     The context of the cross-section from which to load the
     *     cross-section data set.
     *
     * @param mu
     *     Reduced mass of particles in question.
     * */
    static DATACrossSection load(olson_tools::xml::XMLContext & x, const double & mu);
};

}} /* namespace particledb::interaction.*/

#endif // PARTICLEDB_INTERACTION_DATACROSSSECTION_H
