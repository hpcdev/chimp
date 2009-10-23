
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/CrossSection.h>
#include <chimp/interaction/model/Elastic.h>
#include <chimp/interaction/model/InElastic.h>
#include <chimp/interaction/detail/PropertyPtrComparator.h>
#include <chimp/property/name.h>
#include <chimp/property/mass.h>

#include <map>


namespace chimp {
  namespace interaction {


    template < typename options >
    template <class RnDB>
    std::ostream & Equation<options>::print( std::ostream & out,
                                             const RnDB & db ) const {
      Input::print(out,db) << "  -->  ";
      Output::print(out,db);
      return out;
    }


    template < typename options >
    template <class RnDB>
    inline Equation<options>
    Equation<options>::load( const xml::Context & x,
                             const std::string & Eq,
                             const RnDB & db ) {
      return
        Equation::load(x.find("//Interaction[string(Eq)='" + Eq + "']"), db);
    }


    template < typename options >
    template <class RnDB>
    Equation<options>
    Equation<options>::load( const xml::Context & x, const RnDB & db ) {
      using std::string;
      using property::mass;
      using property::name;


      typedef std::map<
        const typename RnDB::Properties *,
        int,
        detail::PropertyPtrComparator
      > equation_elements;
      typedef typename equation_elements::iterator PIter;

      equation_elements in, out;
      int n_in = 0, n_out = 0;

      xml::Context::list xl = x.eval("Eq/In/T");
      for (xml::Context::list::iterator i = xl.begin(); i != xl.end(); ++i ) {
        string particle_name = i->query<string>("P");
        int n = i->query<int>("n",1);

        n_in += n;

        const typename RnDB::Properties * p = &( db[particle_name] );

        PIter j = in.find(p);
        if ( j != in.end() )
          j->second += n;
        else
          in.insert( std::make_pair(p, n) );
      }

      xl = x.eval("Eq/Out/T");
      for (xml::Context::list::iterator i = xl.begin(); i != xl.end(); ++i) {
        string particle_name = i->query<string>("P");
        int n = i->query<int>("n", 1);

        n_out += n;

        const typename RnDB::Properties * p = &( db[particle_name] );

        PIter j = out.find(p);
        if ( j != out.end() )
          j->second += n;
        else
          out.insert( std::make_pair(p, n) );
      }


      Equation retval;

      if (n_in != 2) {
        throw xml::error("Only interactions with binary inputs currently supported");
      }

      {
        PIter i = in.begin();
        int n = i->second;

        retval.A = Term(db.findParticleIndx( i->first->name::value ));
        if ( !(--n) )
          ++i;
        retval.B = Term(db.findParticleIndx( i->first->name::value ));
      }

      retval.set_mu_AB(db);

      /* set the output. */
      for ( PIter i = out.begin(); i != out.end(); ++i ) {
        Term it( db.findParticleIndx(i->first->name::value), i->second );

        if (it.type == -1)
          throw xml::error(
            "Cannot load equation with unknown outputs. "
            "please add outputs to RuntimeDB instance first."
          );

        retval.items.push_back(it);
      }

      {
        /* Determine which type of interaction we are dealing with and
         * instantiate the implementation of the interaction. */

        typedef interaction::model::Elastic<options> elastic;
        typedef interaction::model::InElastic<options> inelastic;

        string i_type = inelastic::label;
        if (in == out)
          i_type = elastic::label;

        i_type = x.query<string>( "@type", i_type );

        typedef typename RnDB::InteractionRegistry::const_iterator IRIter;
        IRIter i = db.interaction_registry.find(i_type);

        if ( i != db.interaction_registry.end() ) {
          retval.interaction.reset(
            i->second->new_load( x, static_cast<const Input&>(retval), db )
          );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml::error(
            "Could not determine interaction type found for interaction '" + Eq + '\'' );
        }
      }



      {
        /* now, instantiate the child CrossSection object with the correct
         * type. */
        xml::Context cs_x = x.find("cross_section");
        string cs_type = cs_x.query<string>("@type");

        typedef typename RnDB::CrossSectionRegistry::const_iterator CSRIter;
        CSRIter i = db.cross_section_registry.find(cs_type);

        if ( i != db.cross_section_registry.end() ) {
          retval.cs.reset( i->second->new_load( cs_x, retval.mu_AB ) );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml::error(
            "cross section type '"+cs_type+
            "' not found for interaction '" + Eq + '\'' );
        }
      }

      return retval;
    }

    /** Effective radius.  Required by octree::Octree and dsmc::ParticleNode. */
    template < typename T >
    double inline effectiveRadius( const interaction::Equation<T> & eq,
                                   const double & v_relative ) {
      return effectiveRadius( *eq.cs, v_relative );
    }

  }/* namespace particldb::interaction */
}/* namespace particldb */
