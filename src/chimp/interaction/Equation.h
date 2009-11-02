/** \file
 * Declaration of interaction::Equation class.
 * */

#ifndef chimp_interaction_Equation_h
#define chimp_interaction_Equation_h

#include <chimp/interaction/Term.h>
#include <chimp/interaction/Input.h>
#include <chimp/interaction/model/Base.h>
#include <chimp/interaction/CrossSection.h>

#include <olson-tools/xml/Doc.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <ostream>
#include <string>


namespace chimp {
  namespace xml = olson_tools::xml;
  using boost::shared_ptr;

  namespace interaction {

    /** Detailed balanced equation. */
    template < typename options >
    struct Equation : Input, Output {
      /* TYPEDEFS */
      /** A vector of equations. */
      typedef std::vector<Equation> list;

      /** Vector of Terms. */
      typedef std::vector<Term>  TermList;



      /* MEMBER STORAGE */
      /** The list of products resulting from this equation. */
      TermList products;

      /** The CrossSection instance that this interaction provides.  */
      shared_ptr<CrossSection> cs;

      /** The interaction model used by this interaction. */
      shared_ptr< model::Base<options> > interaction;



      /* MEMBER FUNCTIONS */
      /** Print the full equation. */
      template < typename RnDB >
      inline std::ostream & print( std::ostream & out, const RnDB & db ) const;

      /** Attempt to load an equation into an Equation instance based on the
       * string representation. */
      template < typename RnDB >
      static inline Equation load( const xml::Context & x,
                                   const std::string & Eq,
                                   const RnDB & db );

      /** Attempt to load an equation from the (assumed) appropriate XML node in
       * an XML document. */
      template < typename RnDB >
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
