
#ifndef particledb_interaction_Output_h
#define particledb_interaction_Output_h

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
    struct Output {
      struct item {
        int n;
        int type;
      };
      typedef std::vector<item> item_list;

      item_list items;

      shared_ptr<CrossSection> cs;
      shared_ptr<model::Base> interaction;

      struct name_mass {
        const property::mass & mass;
        const property::name & name;

        name_mass( const property::mass & m,
                   const property::name & n )
          : mass(m), name(n) {}

        bool operator<(const name_mass & rhs) const {
          return this->mass.value < rhs.mass.value;
        }
      };
      typedef std::multiset<name_mass> printset;

      template <class RnDB>
      std::ostream & print(std::ostream & out, const RnDB & db) const {
        typedef item_list::const_iterator CIter;
        printset ps;
        for ( CIter i = items.begin(); i!=items.end(); ++i ) {
          const property::mass & m = db[i->type];
          const property::name & n = db[i->type];
          ps.insert(name_mass(m,n));
        }

        const char * plus = "+";
        const char * sep = "";
        for ( printset::iterator i = ps.begin(); i!=ps.end(); ++i ) {
          out << sep << '(' << i->name.value << ")";
          sep = plus;
        }
        return out;
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
