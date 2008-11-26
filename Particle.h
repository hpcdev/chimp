
#ifndef PARTICLEDB_PARTICLE_H
#define PARTICLEDB_PARTICLE_H


#include <string>

#include "XMLDoc.h"

namespace particledb { namespace Particle {
    namespace property {
        static const char * property_names[] = {
            "@name",
            "mass", 
            "charge"
        };

        enum PROPERTY_INDX {
            NAME=0,
            MASS=1,
            CHARGE=2
        };

        template <class T> struct null_val { static T value; };
        template <class T> T null_val<T>::value = T(0);
        template    <>     std::string null_val<std::string>::value;

        namespace PHYS {
            enum ID {
                NONE,
                KG,
                M
            };

            template <class T, enum ID id>
            struct check {
                check(const T& t) : value(t) {}
                T value;
            };

        }
    }}

    namespace xml {
        //using namespace Particle::property::PHYS;
        namespace ppp = Particle::property::PHYS;
        template<>
        template<class T, enum ppp::ID id>
        struct parser< ppp::check<T,id> > {
            static ppp::check<T,id> parse(const XMLContext & x) {
                return x.parse<T>();
            }
        };

        template<>
        struct parser< ppp::check<double,ppp::KG> > {
            static ppp::check<double,ppp::KG> parse(const XMLContext & x);
        };

        template<>
        struct parser< ppp::check<double,ppp::M> > {
            static ppp::check<double,ppp::M> parse(const XMLContext & x);
        };
    }

    namespace Particle { namespace property {

        template <class T, enum PROPERTY_INDX P, bool R /*required_on_load*/ = false, enum PHYS::ID id = PHYS::NONE>
        struct property {
            property(const T & val = null_val<T>::value) : value(val) {}
            T value;
            std::ostream & print(std::ostream & out, const std::string & sep) const {
                return out << property_names[P] << ": " << value << sep;
            }
            static T default_value;

            static property load(const xml::XMLContext & x) {
                if (R)
                    return x.query< PHYS::check<T,id> >(property_names[P]).value;
                else
                    return x.query< PHYS::check<T,id> >(property_names[P], default_value).value;
            }
        };

        template <class T, enum PROPERTY_INDX P, bool R, enum PHYS::ID id>
        T property<T,P,R,id>::default_value = null_val<T>::value;

        /** A simple null property with no storage requirements. */
        template <unsigned int id = 0>
        struct NullProp {
            typedef NullProp super;
            std::ostream & print(std::ostream & out, const std::string & sep) const { return out; }
            template <class DBnode>
            static inline NullProp load(const DBnode & x) { return NullProp(); }
        };

        template <class P0,
                  class P1 = NullProp<0>,
                  class P2 = NullProp<1>,
                  class P3 = NullProp<2>,
                  class P4 = NullProp<3>,
                  class P5 = NullProp<4>,
                  class P6 = NullProp<5>,
                  class P7 = NullProp<6>,
                  class P8 = NullProp<7>,
                  class P9 = NullProp<8>
        >
        struct Add : P0, P1, P2, P3, P4, P5, P6, P7, P8, P9 {
            inline std::ostream & print(std::ostream & out) const {
                P0::super::print(out,", ");
                P1::super::print(out,", ");
                P2::super::print(out,", ");
                P3::super::print(out,", ");
                P4::super::print(out,", ");
                P5::super::print(out,", ");
                P6::super::print(out,", ");
                P7::super::print(out,", ");
                P8::super::print(out,", ");
                P9::super::print(out,", ");
                return out;
            }

            template <class DBnode>
            static Add load(const DBnode & x) {
                Add t;
                t.P0::super::operator=(P0::load(x));
                t.P1::super::operator=(P1::load(x));
                t.P2::super::operator=(P2::load(x));
                t.P3::super::operator=(P3::load(x));
                t.P4::super::operator=(P4::load(x));
                t.P5::super::operator=(P5::load(x));
                t.P6::super::operator=(P6::load(x));
                t.P7::super::operator=(P7::load(x));
                t.P8::super::operator=(P8::load(x));
                t.P9::super::operator=(P9::load(x));
                return t;
            }
        };

        template <class P0, class P1, class P2, class P3, class P4,
                  class P5, class P6, class P7, class P8, class P9
        >
        inline std::ostream & operator<<(
            std::ostream & out,
            const Add<P0,P1,P2,P3,P4,P5,P6,P7,P8,P9> & agg) {
                return agg.print(out);
        }

        /* typedefs for various properties. */
        typedef property<std::string, NAME,    true          > name_t;
        typedef property<double,      MASS,   false, PHYS::KG> mass_t;
        typedef property<int,         CHARGE                 > charge_t;

        /* struct defs for previously typedefed properties.  We do this in
         * this manner to allow the user to later reference the various
         * properties by their name, such as:
         * name::value, mass::value, etc.  Otherwise, the user would have
         * to reference the values by P0::value, P1::value, etc. */
        struct name   : name_t   { typedef name_t   super; };
        struct mass   : mass_t   { typedef mass_t   super; };
        struct charge : charge_t { typedef charge_t super; };

        /** Representative size of this type of particle. */
        struct size {
            typedef size super;
            int value;
            size(const int & sz = 1) : value(sz) {}
            std::ostream & print(std::ostream & out,
                                 const std::string & sep) const {
                return out << "size: " << value << sep;
            }
            static inline size load(const xml::XMLContext & x) { return size(); }
        };

    }

    typedef property::Add<
        property::name,
        property::size,
        property::mass,
        property::charge
    > Properties;

}}/*namespace particledb::Particle */

#endif // PARTICLEDB_PARTICLE_H
