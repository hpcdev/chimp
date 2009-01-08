#include "Particle.h"
#include <olson-tools/xml/physical_parse.h>

namespace particledb { namespace Particle { namespace property { namespace PHYS {
    template    <>     std::string null_val<std::string>::value = "";

    void parse_item(check<double,KG> & out, const xml::XMLContext & x) {
        using physical::Quantity;
        using physical::unit::kg;
        out = x.parse<Quantity>().assertMatch(kg).coeff;
    }
    
    void parse_item(check<double,M> & out, const xml::XMLContext & x) {
        using physical::Quantity;
        using physical::unit::m;
        out = x.parse<Quantity>().assertMatch(m).coeff;
    }
}}}}/* namespace particledb */
