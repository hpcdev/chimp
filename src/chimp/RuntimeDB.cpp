// -*- c++ -*-
// $Id$

#  include <chimp/RuntimeDB.h>
#  include <chimp/physical_calc.h>
#  include <chimp/interaction/model/Elastic.h>
#  include <chimp/interaction/model/InElastic.h>
#  include <chimp/interaction/model/VSSElastic.h>
#  include <chimp/interaction/filter/EqIO.h>
#  include <chimp/interaction/filter/Elastic.h>
#  include <chimp/interaction/VHSCrossSection.h>
#  include <chimp/interaction/DATACrossSection.h>

#  include <math.h>
#  include <ostream>
#  include <fstream>
#  include <cfloat>
#  include <set>
#  include <string>
#  include <algorithm>



namespace chimp {

  template < typename T >
  RuntimeDB<T>::RuntimeDB(const std::string & xml_doc) : xmlDb(xml_doc) {
    /* Let's make sure that the calculator is prepared. */
    prepareCalculator(xmlDb);


    /* register the library-provided CrossSection functors. */
    cross_section_registry["vhs"].reset(new interaction::VHSCrossSection);
    cross_section_registry["data"].reset(new interaction::DATACrossSection);


    /* register the library-provided Interaction functors. */
    typedef interaction::model::Elastic<options> elastic;
    typedef interaction::model::VSSElastic<options> vsselastic;
    typedef interaction::model::InElastic<options> inelastic;

    interaction_registry[elastic::label].reset( new elastic);
    interaction_registry[vsselastic::label].reset( new vsselastic);
    interaction_registry[inelastic::label].reset( new inelastic);

    /* set up the default interaction filter. */
    filter.reset( new interaction::filter::Elastic );
  }


  template < typename T >
  void RuntimeDB<T>::initBinaryInteractions() {
    /* first thing we do is to sort the particle property entries by mass and
     * name. */
    std::sort(props.begin(), props.end(), property::Comparator());

    interactions.resize(props.size());

    for (unsigned int A = 0; A < props.size(); ++A) {
      for (unsigned int B = A; B < props.size(); ++B) {
        using std::string;
        using property::name;
        const string & n_A = props[A].name::value;
        const string & n_B = props[B].name::value;

        using interaction::filter::EqTerm;
        using interaction::filter::EqTermSet;
        EqTermSet in_eq_set;
        if ( A == B ) {
          in_eq_set.insert( EqTerm(n_A,2) );
        } else {
          in_eq_set.insert( EqTerm(n_A,1) );
          in_eq_set.insert( EqTerm(n_B,1) );
        }

        /* get the set of all interactions with the correct inputs. */
        xml::Context::list xl = xmlDb.eval(
          "//Interaction/" + get_xpath_query("In", in_eq_set) );
        xml::Context::set  xs( xl.begin(), xl.end() );

        /* now filter the interactions to get the desired subset. */
        xs = filter->filter( xs );

        /* first instantiate the (A,B)th interactions */
        Set & set = interactions(A,B);
        set.lhs.setInput(*this,A,B);

        /* add each of the allowed interactions to the new set. */
        for (xml::Context::set::iterator k = xs.begin(); k != xs.end(); k++)
          set.rhs.push_back(Set::Equation::load(*k,*this));
      }
    }
  }


  template < typename T >
  inline typename std::vector<typename RuntimeDB<T>::Properties>::const_iterator
  RuntimeDB<T>::findParticle(const std::string & name) const {
    typename prop_list::const_iterator i = props.begin();
    for (; i != props.end(); i++) {
      property::name n = (*i);
      if (name == n.value)
        break;
    }
    return i;
  }


  template < typename T >
  inline typename std::vector<typename RuntimeDB<T>::Properties>::iterator
  RuntimeDB<T>::findParticle(const std::string & name) {
    typename prop_list::iterator i = props.begin();
    for (; i != props.end(); i++) {
      property::name n = (*i);
      if (name == n.value)
        break;
    }
    return i;
  }


  template < typename T >
  inline int RuntimeDB<T>::findParticleIndx(const std::string & name) const {
    typename prop_list::const_iterator i = findParticle(name);
    if (i == props.end())
      return -1;
    return i - props.begin();
  }


  template < typename T >
  inline void RuntimeDB<T>::addParticleType(const std::string & name) {
    using xml::Context;
    Context x = xmlDb.root_context.find("//Particle[@name=\"" + name + "\"]");
    addParticleType(x);
  }


  template < typename T >
  inline void RuntimeDB<T>::addParticleType(const xml::Context & x) {
    Properties prop = Properties::load(x);
    addParticleType(prop);
  }


  template < typename T >
  inline void RuntimeDB<T>::addParticleType(const Properties & prop) {
    using property::name;
    const std::string & n = prop.name::value;
    using olson_tools::logger::log_warning;

    /* See if this particle type has already been
     * loaded into the database (we don't want any duplicates). */
    if ( findParticle(n) != props.end() )
      log_warning( "particle type '%s' was previously loaded; will not reload",
                   n.c_str() );
    else
      props.push_back(prop);
  }


  template < typename T >
  inline const typename RuntimeDB<T>::Properties &
  RuntimeDB<T>::operator[] ( const std::string & n ) const {
    typename prop_list::const_iterator i = findParticle(n);
    if (i == props.end())
      throw std::runtime_error("particle type not loaded: '" + n + '\'');
    return  *i;
  }


  template < typename T >
  inline typename RuntimeDB<T>::Properties &
  RuntimeDB<T>::operator[] ( const std::string & n ) {
    typename prop_list::iterator i = findParticle(n);
    if (i == props.end())
      throw std::runtime_error("particle type not loaded: '" + n + '\'');
    return  *i;
  }


  template < typename T >
  inline const typename RuntimeDB<T>::Set &
  RuntimeDB<T>::operator() ( const int & i, const int & j ) const {
    return interactions(i,j);
  }


  template < typename T >
  inline typename RuntimeDB<T>::Set &
  RuntimeDB<T>::operator() ( const int & i, const int & j ) {
    return interactions(i,j);
  }

} /* namespace chimp */

