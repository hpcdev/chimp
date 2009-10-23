/** \file
 * Definition of the left-hand-side of interaction equations.
 * */

#ifndef chimp_interaction_Input_h
#define chimp_interaction_Input_h

#include <chimp/interaction/Term.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>

#include <ostream>
#include <string>


namespace chimp {
  namespace interaction {

    /** Input data type indices.
     * \todo Allow inputs with more than two species.  This is going to be
     * necessary such that three-body recombination equations will be possible. 
     */
    struct Input {
      /* MEMBER STORAGE */
      /** The first Term of the Input portion of an Equation. */
      Term A;

      /** The second Term of the Input portion of an Equation. */
      Term B;

      /** The reduced mass of the two species in these terms. */
      double mu_AB;



      /* MEMBER FUNCTIONS */
      /** The default constructor sets Terms and reduced mass to invalid values.
       * The caller can optionally supply the terms and reduced mass explicitly.
       * */
      Input( const Term & A = Term(),
             const Term & B = Term(),
             const double & mu_AB = 0.0 ) :
        A(A), B(B), mu_AB(mu_AB) {}

      /** A constructor that specifies the A and B terms.  The caller must also
       * provide a reference to a RuntimeDB instance from which masses of the
       * two species can be retrieved. 
       *
       * @param db
       *    The reference to an instance of the RuntimeDB class.
       * @param A
       *    The first term.
       * @param B
       *    The second term.
       */
      template <class RnDB>
      Input( const RnDB & db, const Term & A, const Term & B ) {
        setInput(db,A,B);
      }

      /** Set the terms of the Equation Input and implicitly the reduced mass.
       * This function uses the provided reference to the RuntimeDB class to
       * obtain the species masses and compute the reduced mass. 
       * @param db
       *    The reference to an instance of the RuntimeDB class.
       * @param A
       *    The first term.
       * @param B
       *    The second term.
       */
      template <class RnDB>
      void setInput( const RnDB & db, const Term & A, const Term & B ) {
        this->A = A;
        this->B = B;
        set_mu_AB(db);
      }

      /** Set the reduced mass using the reference to the supplied RuntimeDB
       * class.
       * @param db
       *    The reference to an instance of the RuntimeDB class.
       */
      template <class RnDB>
      void set_mu_AB(const RnDB & db) {
        using property::mass;
        const double & m_A = db[A.type].mass::value;
        const double & m_B = db[B.type].mass::value;
        mu_AB = m_A * m_B / (m_A + m_B);
      }

      /** The stream printer for the Equation Input class.
       * @param out
       *    The stream to which to print.
       * @param db
       *    The reference to an instance of the RuntimeDB class.
       */
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

    /** Less than operation for interaction::Input orders by A and then by B. */
    inline bool operator< ( const Input & lhs, const Input & rhs ) {
      return lhs.A < rhs.A || (lhs.A == rhs.A && lhs.B < rhs.B);
    }

  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_Input_h
