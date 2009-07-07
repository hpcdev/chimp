
#ifndef particledb_Particle_h
#define particledb_Particle_h


#include <olson-tools/xml/XMLDoc.h>

#include <string>


namespace particledb {
  namespace Particle {
    namespace property {
      namespace xml = olson_tools::xml;

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
          check(const T& t = T()) : value(t) {}
          T value;
        };

        template<class T, enum ID id>
        static void parse_item( check<T,id> & out,
                                const xml::XMLContext & x ) {
          out.value = x.parse<T>();
        }

        extern void parse_item( check<double,KG> & out,
                                const xml::XMLContext & x );
        extern void parse_item( check<double,M>  & out,
                                const xml::XMLContext & x );
      }/* namespace particledb::Particle::property::PHYS */


      template < typename T,
                 enum PROPERTY_INDX P,
                 bool R /*required_on_load*/ = false,
                 enum PHYS::ID id = PHYS::NONE >
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

      template < typename T, enum PROPERTY_INDX P, bool R, enum PHYS::ID id>
      T property<T,P,R,id>::default_value = null_val<T>::value;

      /** A simple null property with no storage requirements. */
      template < unsigned int id = 0 >
      struct NullProp {
        typedef NullProp super;

        std::ostream & print( std::ostream & out,
                              const std::string & sep ) const {
          return out;
        }

        template < typename DBnode >
        static inline NullProp load(const DBnode & x) { return NullProp(); }
      };

      template < typename P0,
                 typename P1 = NullProp<0>,
                 typename P2 = NullProp<1>,
                 typename P3 = NullProp<2>,
                 typename P4 = NullProp<3>,
                 typename P5 = NullProp<4>,
                 typename P6 = NullProp<5>,
                 typename P7 = NullProp<6>,
                 typename P8 = NullProp<7>,
                 typename P9 = NullProp<8> >
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

      template < typename P0, typename P1, typename P2, typename P3,
                 typename P4, typename P5, typename P6, typename P7,
                 typename P8, typename P9 >
      inline std::ostream & operator<<
        ( std::ostream & out, const Add<P0,P1,P2,P3,P4,P5,P6,P7,P8,P9> & agg ) {
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
        double value;
        size(const double & sz = 1) : value(sz) {}
        std::ostream & print( std::ostream & out,
                              const std::string & sep ) const {
          return out << "size: " << value << sep;
        }
        static inline size load( const xml::XMLContext & x ) { return size(); }
      };

    }/* namespace particledb::Particle::property */

    typedef property::Add<
      property::name,
      property::size,
      property::mass,
      property::charge
    > Properties;

  }/*namespace particledb::Particle */
}/*namespace particledb */

#endif // particledb_Particle_h
