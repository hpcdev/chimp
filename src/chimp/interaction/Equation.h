/** \file
 * Declaration of interaction::Equation class.
 * */

#ifndef chimp_interaction_Equation_h
#define chimp_interaction_Equation_h

#include <chimp/interaction/Input.h>
#include <chimp/interaction/Output.h>

#include <olson-tools/xml/Doc.h>

#include <vector>
#include <ostream>
#include <string>


namespace chimp {
  namespace xml = olson_tools::xml;

  namespace interaction {

    /** Detailed balanced equation. */
    template < typename options >
    struct Equation : Input, interaction::Output<options> {
      /* TYPEDEFS */
      typedef interaction::Output<options> Output;

      /** A vector of equations. */
      typedef std::vector<Equation> list;




      /* MEMBER FUNCTIONS */
      /** Print the full equation. */
      template <class RnDB>
      inline std::ostream & print( std::ostream & out, const RnDB & db ) const;

      /** Attempt to load an equation into an Equation instance based on the
       * string representation. */
      template <class RnDB>
      static inline Equation load( const xml::Context & x,
                                   const std::string & Eq,
                                   const RnDB & db );

      /** Attempt to load an equation from the (assumed) appropriate XML node in
       * an XML document. */
      template <class RnDB>
      static Equation load( const xml::Context & x, const RnDB & db );
    };

    /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
    template < typename T >
    double inline effectiveRadius( const interaction::Equation<T> & eq,
                                   const double & v_relative );

  }/* namespace particldb::interaction */
}/* namespace particldb */

#include <chimp/interaction/Equation.cpp>

#endif // chimp_interaction_Equation_h
