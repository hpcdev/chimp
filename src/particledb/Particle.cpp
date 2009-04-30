#include "Particle.h"
#include <olson-tools/xml/physical_parse.h>

namespace particledb { namespace Particle { namespace property {
    template    <>     std::string null_val<std::string>::value = "";
namespace PHYS {
    void parse_item(check<double,KG> & out, const xml::XMLContext & x) {
        using runtime::physical::Quantity;
        using runtime::physical::unit::kg;
        x.parse<Quantity>().assertMatch(kg).getCoeff(out.value);
    }
    
    void parse_item(check<double,M> & out, const xml::XMLContext & x) {
        using runtime::physical::Quantity;
        using runtime::physical::unit::m;
        x.parse<Quantity>().assertMatch(m).getCoeff(out.value);
    }
} /* namespace PHYS */
}}}/* namespace particledb::Particle::property */
