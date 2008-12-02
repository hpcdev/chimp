#ifndef PARTICLEDB_PHYSICAL_PARSE_H
#define PARTICLEDB_PHYSICAL_PARSE_H

#include "XMLDoc.h"
#include "data_set.h"
#include <olson-tools/physical/calc/infix.h>

namespace particledb {
    
    namespace xml {
    template <>
    struct parser< physical::Quantity > {
        static physical::Quantity parse(const XMLContext & x) {
            using physical::calc::InfixCalc;
        
            if( x.node->type != XML_ELEMENT_NODE &&
                x.node->type != XML_ATTRIBUTE_NODE &&
                x.node->type != XML_TEXT_NODE)
                throw xml_error("unknown xml node type");
        
            char * bob = (char*)xmlNodeGetContent(x.node);
            physical::Quantity q = InfixCalc::instance().parse(bob);
            free(bob);
            return q;
        }
    };
    } /* namespace xml */


    template <class C, class D, class InIter>
    data_set<C,D> convert_data_set(
        InIter i, const InIter & f,
        const std::pair<physical::Quantity,physical::Quantity> & units) {
        data_set<C,D> retval;
        for (; i != f; i++)
            retval.insert(
                std::make_pair<C,D>(
                    i->first.assertMatch(units.first).coeff,
                    i->second.assertMatch(units.second).coeff
                )
            );
        return retval;
    }

} /* namespace particledb */
#endif // PARTICLEDB_PHYSICAL_PARSE_H
