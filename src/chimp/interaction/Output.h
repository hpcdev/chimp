
#ifndef chimp_interaction_Output_h
#define chimp_interaction_Output_h

#include <chimp/interaction/Term.h>
#include <chimp/interaction/model/Base.h>
#include <chimp/interaction/CrossSection.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <ostream>
#include <string>


namespace chimp {
  using boost::shared_ptr;

  namespace interaction {

    /** Output information of interactions. */
    template < typename options >
    struct Output {
      /* TYPEDEFS */
      typedef std::vector<Term> term_list;


      /* MEMBER STORAGE */
      term_list items;
      shared_ptr<CrossSection> cs;
      shared_ptr< model::Base<options> > interaction;


      /* MEMBER FUNCTIONS */
      /** Stream printer. */
      template <class RnDB>
      std::ostream & print(std::ostream & out, const RnDB & db) const {
        typedef typename term_list::const_iterator CIter;
        Term::printset ps;
        for ( CIter i = items.begin(); i!=items.end(); ++i )
          ps.add(*i, db);

        return ps.print(out, db);
      }

      /** Compute the cross section.
       * @param v_relative
       *     The relative velocity of the two particles in question.
       * */
      double cross_section(const double & v_relative) const {
        return cs->operator()(v_relative);
      }

      /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
      double effective_radius(const double & v_relative) const {
        return cs->effective_radius(v_relative);
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // chimp_interaction_Output_h
