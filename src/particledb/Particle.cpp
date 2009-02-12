#include "Particle.h"
#include <olson-tools/xml/physical_parse.h>

namespace particledb { namespace Particle { namespace property {
    template    <>     std::string null_val<std::string>::value = "";
namespace PHYS {
    void parse_item(check<double,KG> & out, const xml::XMLContext & x) {
        using runtime::physical::Quantity;
        using runtime::physical::unit::kg;
        out = x.parse<Quantity>().assertMatch(kg).coeff;
    }
    
    void parse_item(check<double,M> & out, const xml::XMLContext & x) {
        using runtime::physical::Quantity;
        using runtime::physical::unit::m;
        out = x.parse<Quantity>().assertMatch(m).coeff;
    }
} /* namespace PHYS */
}}}/* namespace particledb::Particle::property */
