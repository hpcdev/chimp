
#ifndef particledb_interaction_Input_h
#define particledb_interaction_Input_h

#include <particledb/property/name.h>
#include <particledb/property/mass.h>

#include <ostream>
#include <string>


namespace particledb {
  namespace interaction {

    /** Input data type indices. */
    struct Input {
      int A;
      int B;
      double mu_AB;

      Input( const int & A = 0,
             const int & B = 0,
             const double & mu_AB = 0.0 ) :
        A(A), B(B), mu_AB(mu_AB) {}

      template <class RnDB>
      Input( const RnDB & db, const int & A = 0, const int & B = 0 ) {
        setInput(db,A,B);
      }

      template <class RnDB>
      void setInput( const RnDB & db, const int & A = 0, const int & B = 0 ) {
        this->A = A;
        this->B = B;
        set_mu_AB(db);
      }

      template <class RnDB>
      void set_mu_AB(const RnDB & db) {
        using property::mass;
        const double & m_A = db[A].mass::value;
        const double & m_B = db[B].mass::value;
        mu_AB = m_A * m_B / (m_A + m_B);
      }

      template <class RnDB>
      std::ostream & print( std::ostream & out, const RnDB & db ) const {
        using std::string;
        using property::mass;
        using property::name;
        const string & n_A = db[A].name::value;
        const string & n_B = db[B].name::value;
        const double & m_A = db[A].mass::value;
        const double & m_B = db[B].mass::value;

        if ( A != B ) {
          const string * n0 = &n_A;
          const string * n1 = &n_B;
          if (m_A > m_B) {
            n0 = &n_B;
            n1 = &n_A;
          }

          out << '(' << (*n0) << ")"
                "+(" << (*n1) << ')';
        } else
          out << '(' << n_A << ")";

        return out;
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // particledb_interaction_Input_h
