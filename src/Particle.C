#include "Particle.h"
#include "physical_parse.h"

namespace particledb {
    
    namespace Particle { namespace property {
        template    <>     std::string null_val<std::string>::value = "";
    }}

    namespace xml {
    using namespace Particle::property::PHYS;

check<double,KG> parser< check<double,KG> >::parse(const XMLContext & x) {
    using physical::Quantity;
    using physical::unit::kg;
    return x.parse<Quantity>().assertMatch(kg).coeff;
}

check<double,M> parser< check<double,M> >::parse(const XMLContext & x) {
    using physical::Quantity;
    using physical::unit::m;
    return x.parse<Quantity>().assertMatch(m).coeff;
}

}}/* namespace particledb::xml */
