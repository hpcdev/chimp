
#ifndef particledb_interaction_filter_Input_h
#define particledb_interaction_filter_Input_h

#include <particledb/interaction/filter/Base.h>

#include <string>
#include <sstream>
#include <set>

namespace particledb {
  namespace interaction {
    namespace filter {

      /** Specification of a single term in an equation. */
      struct EqTerm {
        std::string name;
        int n;

        EqTerm( const std::string & name, const int & n = 1 )
          : name(name), n(n) { }

        EqTerm( const char * name, const int & n = 1 )
          : name(name), n(n) { }
      };

      /** The comparator for Eq terms sorts by name only.  The reason for this
       * is to ensure that we can only have one entry for each particle type.
       * The side-effect of this comparator is obviously that any subsequent
       * insertion of an EqTerm will override a previous entry with the same
       * name. */
      struct EqTermComparator {
        bool operator()( const EqTerm & lhs, const EqTerm & rhs ) {
          return lhs.name < rhs.name;
        }
      };

      /** Set of EqTerm instances using the EqTermComparator. */
      typedef std::set<EqTerm, EqTermComparator> EqTermSet;

      /** Translate a set of Equation terms into xpath query.  This function
       * assumes the context is at the Interaction level.  The context returned
       * by this query is also at the Interaction level.
       */
      inline std::string get_xpath_query( const std::string & EqIO,
                                          const EqTermSet & set ) {
        std::ostringstream query;
        query << "Eq[count("<<EqIO<<"/T) = " << set.size() << "]/" << EqIO;

        for ( EqTermSet::const_iterator i = set.begin(); i != set.end(); ++i ) {
          if ( i->n == 0 )
            continue;

          query << "/T[string(P)='"<<i->name<<"' and ";

          if ( i->n == 1 )
            query << "(number(n)=1 or string(n)='')]/..";
          else
            query << "number(n)="<<i->n<<"]/..";
        }

        query << "/../..";

        return query.str();
      }



      /** Filters an Equation based on a set of Input/Output terms. */
      class EqIO : public Base {
        /* TYPEDEFS */
      public:
        enum EqIOId {
          IN,
          OUT
        };


        /* MEMBER STORAGE */
      public:
        EqTermSet terms;

      private:
        EqIOId m_id;
        std::string m_id_name;

        /* MEMBER FUNCTIONS */
      public:
        /** Default constructor does not initialize the xpath filter string. */
        EqIO( const EqIOId & id ) : terms() { setIOId(id); }

        /** Constructor with up to three input terms specified. */
        EqIO( const EqIOId & id,
              const EqTerm & A,
              const EqTerm & B = EqTerm("",0),
              const EqTerm & C = EqTerm("",0) ) {
          setIOId(id);

          if ( A.n )
            terms.insert( A );

          if ( B.n )
            terms.insert( B );

          if ( C.n )
            terms.insert( C );

          set_xpath_query();
        }

        /** Constructor with the set of terms. */
        EqIO( const std::set<EqTerm> & terms )
          : terms(terms.begin(),terms.end()) {
          set_xpath_query();
        }

        const EqIOId & getIOId() const { return m_id; }

        void setIOId( const EqIOId & id ) {
          m_id = id;
          m_id_name = id == EqIO::IN ? "In" : "Out";
        }

        /** set the xpath query to validate the nodes. */
        void set_xpath_query() {
          xpath_query = get_xpath_query(m_id_name, terms);
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_filter_Input_h
