
#include <chimp/interaction/Equation.h>
#include <chimp/interaction/CrossSection.h>
#include <chimp/interaction/model/Elastic.h>
#include <chimp/interaction/model/InElastic.h>
#include <chimp/interaction/detail/sort_terms.h>
#include <chimp/property/name.h>


namespace chimp {
  namespace interaction {


    template < typename options >
    template < typename RnDB >
    std::ostream & Equation<options>::print( std::ostream & out,
                                             const RnDB & db ) const {
      Input::print(out,db) << "  -->  ";
      printTerms( products.begin(), products.end(), out, db );
      return out;
    }


    template < typename options >
    template < typename RnDB >
    inline Equation<options>
    Equation<options>::load( const xml::Context & x,
                             const std::string & Eq,
                             const RnDB & db ) {
      return
        Equation::load(x.find("//Interaction[string(Eq)='" + Eq + "']"), db);
    }


    template < typename options >
    template < typename RnDB >
    Equation<options>
    Equation<options>::load( const xml::Context & x, const RnDB & db ) {
      using property::name;

      typedef typename detail::makeSortedTermMap<RnDB>::type SortedElements;
      typedef typename SortedElements::iterator SEIter;

      detail::sorted_equation_elements in, out;

      /* Load up the reactants and products and sort them according to
       * chimp::property::Comparator. */
      int n_in  = detail::loadAndSortTerms( x.eval("Eq/In/T"),  in,  db );
      int n_out = detail::loadAndSortTerms( x.eval("Eq/Out/T"), out, db );

      Equation retval;

      /* FIXME:  allow nore than two inputs and store the input terms in a list,
       * just as is done for the products. */
      if (n_in != 2) {
        throw xml::error("Only interactions with binary inputs currently supported");
      }

      {
        SEIter i = in.begin();
        int n = i->second;

        retval.A = Term(db.findParticleIndx( i->first->name::value ));
        if ( !(--n) )
          ++i;
        retval.B = Term(db.findParticleIndx( i->first->name::value ));
      }

      /* FIXME:  This was done so that we could pass it to the cross sections
       * below.  We should really just pass a reference to the database and this
       * Equation instance so the cross sections can just ask for what they
       * need through some part of the Equation interface. */
      retval.set_mu_AB(db);

      /* set the output. */
      for ( SEIter i = out.begin(); i != out.end(); ++i ) {
        Term it( db.findParticleIndx(i->first->name::value), i->second );

        if (it.species == -1)
          throw xml::error(
            "Cannot load equation with unknown outputs. "
            "please add outputs to RuntimeDB instance first."
          );

        retval.products.push_back(it);
      }

      {
        /* Determine which model of interaction we are dealing with and
         * instantiate the implementation of the interaction. */

        typedef interaction::model::Elastic<options> elastic;
        typedef interaction::model::InElastic<options> inelastic;
        using std::string;

        string i_model = inelastic::label;
        if (in == out)
          i_model = elastic::label;

        i_model = x.query<string>( "@model", i_model );

        typedef typename RnDB::InteractionRegistry::const_iterator IRIter;
        IRIter i = db.interaction_registry.find(i_model);

        if ( i != db.interaction_registry.end() ) {
          retval.interaction.reset(
            i->second->new_load( x, static_cast<const Input&>(retval), db )
          );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml::error(
            "Could not determine interaction model found for interaction '" + Eq + '\'' );
        }
      }



      {
        /* now, instantiate the child CrossSection object with the correct
         * model. */
        using std::string;

        xml::Context cs_x = x.find("cross_section");
        string cs_model = cs_x.query<string>("@model");

        typedef typename RnDB::CrossSectionRegistry::const_iterator CSRIter;
        CSRIter i = db.cross_section_registry.find(cs_model);

        if ( i != db.cross_section_registry.end() ) {
          retval.cs.reset(
            i->second->new_load( cs_x, static_cast<const Input&>(retval), db )
          );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml::error(
            "cross section model '"+cs_model+
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
