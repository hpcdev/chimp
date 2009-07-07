
#ifndef particledb_interaction_Equation_h
#define particledb_interaction_Equation_h

#include <particledb/interaction/Input.h>
#include <particledb/interaction/Output.h>
#include <particledb/interaction/CrossSection.h>
#include <particledb/Particle.h>

#include <vector>
#include <ostream>
#include <string>


namespace particledb {
  namespace interaction {

    struct Equation : Input, Output {
      typedef std::vector<Equation> list;

      template <class RnDB>
      std::ostream & print( std::ostream & out, const RnDB & db ) const {
        Input::print(out,db) << "->";
        Output::print(out,db);
        return out;
      }

      template <class RnDB>
      static Equation load( const xml::XMLContext & x,
                            const std::string & Eq,
                            const RnDB & db) {
        return
          Equation::load(x.find("//Interaction[string(Eq)='" + Eq + "']"), db);
      }

      template <class RnDB>
      static Equation load( const xml::XMLContext & x, const RnDB & db ) {
        using xml::XMLContext;
        using xml::xml_error;
        using std::string;
        using Particle::property::mass;

        Equation retval;
        XMLContext::list xl = x.eval("Eq/In/P");

        if (xl.size() == 2) {
          XMLContext::list::iterator i = xl.begin();
          retval.A = db.findParticleIndx(i->parse<string>());
          retval.B = db.findParticleIndx((++i)->parse<string>());
          if (retval.A == -1 || retval.B == -1)
            throw xml_error(
              "Cannot load equation with unknown inputs. "
              "please add inputs to RuntimeDB instance first."
            );
        } else if (xl.size() == 1) {
          /* single species elastic collision */
          if (xl.begin()->query<int>("@M") != 2)
            throw xml_error("(too few inputs) Binary interaction must have two inputs");
          retval.A = retval.B = db.findParticleIndx(xl.begin()->parse<string>());
        } else
          throw xml_error("(too many inputs) Binary interaction must have two inputs");

        retval.set_mu_AB(db);

        /* now determine the output particles and their respective
         * multipliers. */
        xl = x.eval("Eq/Out/P");
        for (XMLContext::list::iterator i = xl.begin(); i != xl.end(); i++) {
          Output::item it = {0, db.findParticleIndx(i->parse<string>())};

          if (it.type == -1)
            throw xml_error(
              "Cannot load equation with unknown outputs. "
              "please add outputs to RuntimeDB instance first."
            );

          try {
            it.n = i->query<int>("@M");
          } catch (xml::no_results) {/* ignored */}

          retval.items.push_back(it);
        }

        /* now, instantiate the child CrossSection object with the correct
         * type. */
        XMLContext cs_x = x.find("cross_section");
        string cs_type = cs_x.query<string>("@type");

        if ( db.cross_section_registry.find(cs_type) !=
             db.cross_section_registry.end() ) {
          retval.cs.reset(
            db.cross_section_registry.[cs_type].clone( cs_x, retval.mu_AB )
          );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml_error(
            "cross section type '"+cs_type+
            "' not found for interaction '" + Eq + '\'' );
        }


        /* now, instantiate the child Interaction object with the correct
         * type. */
        string i_type = x.query<string>("@type");

        if ( db.interaction_registry.find(i_type) !=
             db.interaction_registry.end() ) {
          retval.interaction.reset(
            db.interaction_registry.[i_type].clone( XXX, XXX )
          );
        } else {
          string Eq = x.query<string>("Eq");
          throw xml_error(
            "Could not determine interaction type found for interaction '" + Eq + '\'' );
        }

        return retval;
      }
    };

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // particledb_interaction_Equation_h
