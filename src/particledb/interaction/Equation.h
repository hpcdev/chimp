
#ifndef particledb_interaction_Equation_h
#define particledb_interaction_Equation_h

#include <particledb/interaction/Input.h>
#include <particledb/interaction/Output.h>
#include <particledb/interaction/CrossSection.h>
#include <particledb/interaction/model/Elastic.h>
#include <particledb/interaction/model/InElastic.h>
#include <particledb/property/name.h>
#include <particledb/property/mass.h>
#include <particledb/property/Comparator.h>

#include <olson-tools/xml/XMLDoc.h>

#include <vector>
#include <ostream>
#include <string>
#include <map>


namespace particledb {
  namespace xml = olson_tools::xml;

  namespace interaction {

    namespace detail {
      /** Comparator used to sort particles first by mass and then by name. */
      struct PropertyPtrComparator : particledb::property::Comparator {
        typedef particledb::property::Comparator super;

        template < typename Properties >
        bool operator() ( const Properties * lhs, const Properties * rhs ) {
          return super::operator()(*lhs,*rhs);
        }
      };
    }

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
      std::ostream & print( std::ostream & out, const RnDB & db ) const {
        Input::print(out,db) << "  -->  ";
        Output::print(out,db);
        return out;
      }

      /** Attempt to load an equation into an Equation instance based on the
       * string representation. */
      template <class RnDB>
      static Equation load( const xml::XMLContext & x,
                            const std::string & Eq,
                            const RnDB & db ) {
        return
          Equation::load(x.find("//Interaction[string(Eq)='" + Eq + "']"), db);
      }

      /** Attempt to load an equation from the (assumed) appropriate XML node in
       * an XML document. */
      template <class RnDB>
      static Equation load( const xml::XMLContext & x, const RnDB & db ) {
        using xml::XMLContext;
        using xml::xml_error;
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

        XMLContext::list xl = x.eval("Eq/In/T");
        for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); ++i ) {
          std::string particle_name = i->query<string>("P");
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
        for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); ++i) {
          std::string particle_name = i->query<string>("P");
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
          throw xml_error("Only interactions with binary inputs currently supported");
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
            throw xml_error(
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
            throw xml_error(
              "Could not determine interaction type found for interaction '" + Eq + '\'' );
          }
        }



        {
          /* now, instantiate the child CrossSection object with the correct
           * type. */
          XMLContext cs_x = x.find("cross_section");
          string cs_type = cs_x.query<string>("@type");

          typedef typename RnDB::CrossSectionRegistry::const_iterator CSRIter;
          CSRIter i = db.cross_section_registry.find(cs_type);

          if ( i != db.cross_section_registry.end() ) {
            retval.cs.reset( i->second->new_load( cs_x, retval.mu_AB ) );
          } else {
            string Eq = x.query<string>("Eq");
            throw xml_error(
              "cross section type '"+cs_type+
              "' not found for interaction '" + Eq + '\'' );
          }
        }



        return retval;
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // particledb_interaction_Equation_h
