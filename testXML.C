/** \file
 * Simple test for XMLDoc class + physical::Quantity parser.
 * */


#include <iostream>
#include "physical_parse.h"
#include "physical_calc.h"

typedef particledb::data_set<physical::Quantity,physical::Quantity> pqdata_set;

int main(int argc, char **argv) {
    using physical::Quantity;
    using namespace particledb::xml;
    using particledb::data_set;
    using particledb::convert_data_set;
    using physical::constant::eV;
    using physical::unit::m;

    XMLDoc db("particledb.xml");
    prepareCalculator(db);


    /* Evaluate xpath expression */
    for (int i = 1; i < argc; i++) {
        bool raw = false;
        switch(argv[i][0]) {
            case 'q':
                try {
                    std::cout << db.query<Quantity>(argv[i]+1) << std::endl;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 'd':
                try {
                    enum Quantity::PRINT_TYPE old = Quantity::print_type;
                    Quantity::print_type = Quantity::MATH_PRINT;
                    std::cout << db.query<pqdata_set>(argv[i]+1) << std::endl;
                    Quantity::print_type = old;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            default:
                try {
                    std::cout << db.query<physical::Quantity>(argv[i]) << std::endl;
                } catch (physical::exception & e) {
                    std::cout << e.what() << std::endl;
                }
                break;

            case 'r':
                raw = true;
            case 'm':
                XMLContext::list xl = db.eval(argv[i]+1);
                XMLContext::list::iterator i = xl.begin();
                for (; i != xl.end(); i++) {
                    XMLContext & x = (*i);
                    try {
                        if (!raw)
                            std::cout << x.parse<Quantity>() << std::endl;
                        else
                            std::cout << x.text() << std::endl;
                    } catch (physical::exception & e) {
                        std::cout << x.text() << std::endl;
                    }
                }
                break;
        }
    }
    return 0;
}

