
#ifndef chimp_interaction_filter_EqIO_h
#define chimp_interaction_filter_EqIO_h

#include <chimp/interaction/filter/Base.h>

#include <string>
#include <sstream>
#include <set>

namespace chimp {
  namespace interaction {
    namespace filter {

      /** Specification of a single term in an equation. */
      struct EqTerm {
        /** The name of the particle for this term. */
        std::string name;

        /** The multiplicity of this term.
         * \f$ n < 0 \f$ is treated like a wildcard for multiplicity. 
         */
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
      inline bool operator< ( const EqTerm & lhs, const EqTerm & rhs ) {
        return lhs.name < rhs.name;
      }

      /** Set of EqTerm instances using the EqTermComparator. */
      typedef std::set<EqTerm> EqTermSet;

      /** Translate a set of Equation terms into xpath query.  This function
       * assumes the context is at the Interaction level.  The context returned
       * by this query is also at the Interaction level.
       *
       * @returns "" if the EqTermSet was empty.
       */
      inline std::string getXpathQuery( const std::string & EqIO,
                                          const EqTermSet & set ) {
        if ( set.size() == 0u )
          return "";

        std::ostringstream query;
        query << "Eq[count("<<EqIO<<"/T) = " << set.size() << "]/" << EqIO;

        for ( EqTermSet::const_iterator i = set.begin(); i != set.end(); ++i ) {
          if ( i->n == 0 )
            continue;

          query << "/T[string(P)='"<<i->name<<'\'';

          if ( i->n == 1 )
            query << " and (number(n)=1 or string(n)='')]/..";
          else if ( i->n > 1 )
            query << " and number(n)="<<i->n<<"]/..";
          else /* if (i->n < 0):  match all terms regardless of multiplicity. */
            query << "]/..";
        }

        query << "/../..";

        return query.str();
      }



      /** Filters an Equation based on a set of Input/Output terms. */
      class EqIO : public filter::Base {
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
        /** Constructor to specify the EqIOId and optionally the EqTerm set.
         * The xpath_query is set according to the supplied EqTermSet.  */
        EqIO( const EqIOId & id, const EqTermSet & terms = EqTermSet() )
        : terms(terms.begin(), terms.end()) {
          setIOId(id);
          set_xpath_query();
        }

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

        const EqIOId & getIOId() const { return m_id; }

        void setIOId( const EqIOId & id ) {
          m_id = id;
          m_id_name = id == EqIO::IN ? "In" : "Out";

          /* Now make sure that the xpath_query is correct. */
          set_xpath_query();
        }

        /** set the xpath query to validate the nodes. */
        void set_xpath_query() {
          xpath_query = getXpathQuery(m_id_name, terms);
        }
      };

    }/* namespace particldb::interaction::filter */
  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_filter_EqIO_h
