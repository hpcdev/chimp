
#ifndef particledb_property_Generic_h
#define particledb_property_Generic_h


#include <olson-tools/xml/XMLDoc.h>

#include <string>

namespace particledb {
  namespace property {
    namespace xml = olson_tools::xml;

    template < typename T, typename id > struct default_val { static T value; };
    template < typename T, typename id > T default_val<T,id>::value = T(0);

    template < typename id > struct default_val<std::string,id> {
      static std::string value;
    };

    template < typename id > std::string default_val<std::string,id>::value = "";

    namespace PHYS {
      enum ID {
        NONE,
        KG,
        M
      };

      template <class T, enum ID phys_id>
      struct check {
        check(const T& t = T()) : value(t) {}
        T value;
      };

      template<class T, enum ID phys_id>
      static void parse_item( check<T,phys_id> & out,
                              const xml::XMLContext & x ) {
        out.value = x.parse<T>();
      }

      extern void parse_item( check<double,KG> & out,
                              const xml::XMLContext & x );
      extern void parse_item( check<double,M>  & out,
                              const xml::XMLContext & x );
    }/* namespace particledb::property::PHYS */


    /** Generic property type.  This class implements a generic load() function.
     * @param T
     *   Storage type of the property field (int, bool, double, struct, ...).
     * @param id
     *   Provides the xpath/xml name of the child/text node used to query the
     *   actual value of the property.  This should typcially be relative to a
     *   Particle xml node.
     * @param R
     *   Indicates whether the property is required to load successfully.  If
     *   this parameter is false (default), it will be assigned the default.
     * @param phys_id
     *   The physical id of the units that will be expected.  
     * */
    template < typename T,
               typename id,
               bool R /*required_on_load*/ = false,
               enum PHYS::ID phys_id = PHYS::NONE >
    struct Generic {
      /* TYPEDEFS */
      typedef T value_type;


      /* MEMBER STORAGE */
      T value;


      /* MEMBER FUNCTIONS */
      /** Default constructor sets value to default_val<T,id>::value. */
      Generic() : value(default_val<T,id>::value) {}

      /** Constructor which sets the value to the given parameter. */
      Generic(const T & val) : value(val) {}

      /** Stream printer. */
      std::ostream & print(std::ostream & out, const std::string & sep) const {
        return out << id::label << ": " << value << sep;
      }

      /** Load function (loads from xml context. */
      static Generic load(const xml::XMLContext & x) {
        if (R)
          return x.query< PHYS::check<T,phys_id> >(id::label).value;
        else
          return x.query< PHYS::check<T,phys_id> >(
            id::label, default_val<T,id>::value
          ).value;
      }
    };

  }/* namespace particledb::property */
}/*namespace particledb */

#endif // particledb_property_Generic_h
