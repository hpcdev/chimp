
#ifndef particledb_interaction_Term_h
#define particledb_interaction_Term_h

#include <particledb/property/name.h>

#include <ostream>
#include <set>

namespace particledb {
  namespace interaction {
    struct Term {
      /* TYPEDEFS */

      class printset {
        /* TYPEDEFS */
        struct name_mass {
          const property::mass & mass;
          const property::name & name;
          const Term & term;

          name_mass( const property::mass & mass,
                       const property::name & name,
                       const Term & term )
            : mass(mass), name(name), term(term) {}

          bool operator<( const name_mass & rhs ) const {
            return this->mass.value < rhs.mass.value ||
                   ( this->mass.value == rhs.mass.value &&
                     this->name.value < rhs.name.value );
          }
        };

        typedef std::set<name_mass> sT;
        sT s;

      public:
        template < typename RnDB >
        void add( const Term & t, RnDB & db ) {
          const property::mass & mass = db[t.type];
          const property::name & name = db[t.type];
          s.insert(name_mass(mass,name,t));
        }

        template <class RnDB>
        std::ostream & print(std::ostream & out, const RnDB & db) const {
          const char * plus = " + ";
          const char * sep = "";
          for ( typename sT::iterator i = s.begin(); i!= s.end(); ++i ) {
            i->term.print(out << sep, db);
            sep = plus;
          }
          return out;
        }
      };



      /* MEMBER STORAGE */
      int type;
      int n;



      /* MEMBER FUNCTIONS */
      /** Constructor. */
      Term( const int & type = 0,
            const int & n = 1 ) : type(type), n(n) { }

      /** Stream printer. */
      template <class RnDB>
      std::ostream & print(std::ostream & out, const RnDB & db) const {
        const property::name & name = db[type];

        if (n > 1)
          out << n << ' ';
        out << name.value;

        return out;
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // particledb_interaction_Term_h
