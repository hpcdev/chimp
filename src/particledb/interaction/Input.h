
#ifndef particledb_interaction_Input_h
#define particledb_interaction_Input_h

#include <particledb/interaction/Term.h>
#include <particledb/property/name.h>
#include <particledb/property/mass.h>

#include <ostream>
#include <string>


namespace particledb {
  namespace interaction {

    /** Input data type indices. */
    struct Input {
      Term A;
      Term B;
      double mu_AB;

      Input( const Term & A = Term(),
             const Term & B = Term(),
             const double & mu_AB = 0.0 ) :
        A(A), B(B), mu_AB(mu_AB) {}

      template <class RnDB>
      Input( const RnDB & db, const Term & A, const int & B ) {
        setInput(db,A,B);
      }

      template <class RnDB>
      void setInput( const RnDB & db, const Term & A, const Term & B ) {
        this->A = A;
        this->B = B;
        set_mu_AB(db);
      }

      template <class RnDB>
      void set_mu_AB(const RnDB & db) {
        using property::mass;
        const double & m_A = db[A.type].mass::value;
        const double & m_B = db[B.type].mass::value;
        mu_AB = m_A * m_B / (m_A + m_B);
      }

      template <class RnDB>
      std::ostream & print( std::ostream & out, const RnDB & db ) const {
        Term::printset ps;

        if ( A.type == B.type )
          ps.add( Term(A.type, 2), db );
        else
          ps.add(A, db);
          ps.add(B, db);
        return ps.print(out, db);
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_Input_h
