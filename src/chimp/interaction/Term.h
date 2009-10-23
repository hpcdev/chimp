/** \file
 * Definition of the Equation Term.
 * */

#ifndef chimp_interaction_Term_h
#define chimp_interaction_Term_h

#include <chimp/property/name.h>

#include <ostream>
#include <set>

namespace chimp {
  namespace interaction {

    /** A single term in a detailed balanced interaction equation. */
    struct Term {
      /* TYPEDEFS */

      /** A class to prepare a set of terms for printing to a stream using the
       * ordered format (sort by mass and then by name).
       */
      class printset {
        /* TYPEDEFS */
      private:
        /** Structure that is actually sorted--also provides its own correct
         * comparator. */
        struct name_mass {
          const property::mass & mass;
          const property::name & name;
          const Term & term;

          /** name_mass constructor. */
          name_mass( const property::mass & mass,
                     const property::name & name,
                     const Term & term )
            : mass(mass), name(name), term(term) {}

          /** The name_mass comparator orders by mass and then by name. */
          bool operator<( const name_mass & rhs ) const {
            return this->mass.value < rhs.mass.value ||
                   ( this->mass.value == rhs.mass.value &&
                     this->name.value < rhs.name.value );
          }
        };

        /** The name_mass set. */
        typedef std::set<name_mass> sT;


        /* MEMBER STORAGE */
      private:
        /** The set of name_mass items as they are prepared to be printed. */
        sT s;

      public:
        /** Add a term to the printset to prepare for ordered printing.
         * @tparam RunDB
         *    The RuntimeDB type. 
         * @param t
         *    The term to add.
         * @param db
         *    An instance of the RuntimeDB class that can be used to query for
         *    information concerning the particle species of the given term.
         */
        template < typename RnDB >
        void add( const Term & t, RnDB & db ) {
          const property::mass & mass = db[t.type];
          const property::name & name = db[t.type];
          s.insert(name_mass(mass,name,t));
        }

        /** Finally and actually print the set of terms. */
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
      /** The index of the particle type of this term.  This index can be used
       * by with the runtime database to determine information about the
       * particle species. */
      int type;

      /** The multiplicity factor of this term.  For example, if the equation is
       * \verbatim  2 e^- --> 2 e^- \endverbatim
       * then the multiplicity factor
       * for the terms on both sides of the equation is '2'. */
      int n;



      /* MEMBER FUNCTIONS */
      /** Term constructor. */
      Term( const int & type = 0,
            const int & n = 1 ) : type(type), n(n) { }

      /** Term stream printer. */
      template <class RnDB>
      std::ostream & print(std::ostream & out, const RnDB & db) const {
        const property::name & name = db[type];

        if (n > 1)
          out << n << ' ';
        out << name.value;

        return out;
      }
    };

    /** Equals operation for interaction::Term.  */
    inline bool operator== ( const Term & lhs, const Term & rhs ) {
      return lhs.type == rhs.type && lhs.n == rhs.n;
    }

    /** Less than operation for interaction::Term orders by type and then by n.
     */
    inline bool operator< ( const Term & lhs, const Term & rhs ) {
      return lhs.type < rhs.type || (lhs.type == rhs.type && lhs.n < rhs.n);
    }

  }/* namespace chimp::interaction */
}/* namespace chimp */
#endif // chimp_interaction_Term_h
