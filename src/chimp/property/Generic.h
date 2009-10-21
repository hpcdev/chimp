
#ifndef chimp_property_Generic_h
#define chimp_property_Generic_h

#include <chimp/property/PHYS/ID.h>
#include <chimp/property/PHYS/detail/check.h>

#include <olson-tools/xml/Doc.h>

#include <string>

namespace chimp {
  namespace property {
    namespace xml = olson_tools::xml;

    /** A structure which provides the default value of a parameter that has a
     * value type T and the specified properties tag. */
    template < typename T, typename tag > struct default_val { static T value; };
    template < typename T, typename tag > T default_val<T,tag>::value = T(0);

    /** Specialization of default_val where the type is a std::string. */
    template < typename tag > struct default_val<std::string,tag> {
      static std::string value;
    };

    /** Unless the developer specializes the assignment of
     * default_val<std::string,tag>::value, it will be assigned to the empty
     * string. */
    template < typename tag > std::string default_val<std::string,tag>::value = "";

    /** Generic property type.  This class implements a generic load() function.
     * The purpose of this class is just to make it a little easier to define
     * properties to be read in without needing to redefine the load() function.
     * See the property::mass for an example of how to use this class.  
     *
     * FIXME: If I remember right, this class was in part created because I was
     * trying to isolate where runtime::physical actually was seen in the code.
     * This is because the classic Boost.Spirit requires quite a bit of time to
     * compile.  I'm not sure if this was really warranted, especially if the
     * Boost.Spirit 2.1 compiles faster.
     *
     * @param T
     *   Storage type of the property field (int, bool, double, struct, ...).
     * @param tag
     *   A class with a <code>label</code> (char* or std::string) member that
     *   describes the xpath/xml name of the child/text node used to query the
     *   actual value of the property.  This should typcially be relative to a
     *   Particle xml node.  For an example, see property::prop_tag::mass which
     *   is used to specify the xpath query for the mass value relative to a
     *   Particle xml node.  
     * @param R
     *   Indicates whether the property is required to load successfully.  If
     *   this parameter is false (default), it will be assigned the default.<br>
     *   [Default:  false]
     * @param phys_id
     *   The physical id of the units that will be expected.  Generally, the
     *   enum value will represent the SI units that will be compared to the
     *   physical::Quantity read in from xml.  <br>
     *   [Default:  PHYS::NONE]
     * */
    template < typename T,
               typename tag,
               bool R /*required_on_load*/ = false,
               enum PHYS::ID phys_id = PHYS::NONE >
    struct Generic {
      /* TYPEDEFS */
      typedef T value_type;


      /* MEMBER STORAGE */
      T value;


      /* MEMBER FUNCTIONS */
      /** Default constructor sets value to default_val<T,tag>::value. */
      Generic() : value(default_val<T,tag>::value) {}

      /** Constructor which sets the value to the given parameter. */
      Generic(const T & val) : value(val) {}

      /** Stream printer. */
      std::ostream & print(std::ostream & out, const std::string & sep) const {
        return out << tag::label << ": " << value << sep;
      }

      /** Load function (loads from xml context. */
      static Generic load(const xml::Context & x) {
        if (R)
          return x.query< PHYS::detail::check<T,phys_id> >(tag::label).value;
        else
          return x.query< PHYS::detail::check<T,phys_id> >(
            tag::label, default_val<T,tag>::value
          ).value;
      }
    };

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_Generic_h
