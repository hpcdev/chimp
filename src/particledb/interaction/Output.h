
#ifndef particledb_interaction_Output_h
#define particledb_interaction_Output_h

#include <particledb/interaction/Term.h>
#include <particledb/interaction/model/Base.h>
#include <particledb/interaction/CrossSection.h>
#include <particledb/property/name.h>
#include <particledb/property/mass.h>

#include <boost/shared_ptr.hpp>

#include <vector>
#include <ostream>
#include <string>


namespace particledb {
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
        return cs->cross_section(v_relative);
      }

      /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
      double effective_radius(const double & v_relative) const {
        return cs->effective_radius(v_relative);
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // particledb_interaction_Output_h
