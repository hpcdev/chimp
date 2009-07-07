
#ifndef particledb_interaction_Basic_h
#define particledb_interaction_Basic_h

#include <string>


namespace particledb {
  namespace interaction {

    /** Determine a unique set of interactions. */
    template < typename XMLContextList_in >
    static inline xml::XMLContext::set filter_interactions(
                 const XMLContextList_in & xl_in,
                 const std::set<std::string> & allowed_equations ) {
      typedef typename XMLContextList_in::const_iterator InCIter;
      using xml::XMLContext;
      using std::string;

      XMLContext::set xl_out;
      for (InCIter i = xl_in.begin(); i!= xl_in.end(); i++) {
        if (allowed_equations.find(i->template query<string>("Eq"))
            != allowed_equations.end())
        xl_out.insert(*i);
      }

      return xl_out;
    }
            

    /** Find all interactions where A and B are inputs.
     * NOTE:  According to the standard Equation format in the xml file, A and
     * B should be in order of mass, with mass(A) < mass(B).
     * B should be in order of mass, with mass(A) < mass(B).
     * This function does not reorder A and B.
     * */
    static inline xml::XMLContext::list find_all_interactions(
            const xml::XMLContext & x,
            const std::string & A,
            const std::string & B ) {
      std::string Eq_in;
      if (A == B)
        Eq_in = '(' + A + ')';
      else
        Eq_in = '(' + A + ")+(" + B + ')';
      /* find all equations that match the given inputs. */
      return x.eval("//Interaction[string(Eq/In)='" + Eq_in + "']");
    }

    /** Find all elastic interactions where A and B are inputs and outputs.
     * NOTE:  According to the standard Equation format in the xml file, A and
     * B should be in order of mass, with mass(A) < mass(B).
     * This function does not reorder A and B.
     * */
    static inline xml::XMLContext::list find_elastic_interactions(
            const xml::XMLContext & x,
            const std::string & A,
            const std::string & B) {
      std::string Eq;
      if (A == B)
        Eq = '(' + A + ")->(" + A + ')';
      else
        Eq = '(' + A + ")+(" + B + ")->(" + A + ")+(" + B + ')';
      /* find all equations that match the given inputs. */
      return x.eval("//Interaction[string(Eq)='" + Eq + "']");
    }

  }/* namespace particldb::interaction */
}/* namespace particldb */
#endif // particledb_interaction_Basic_h
