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
#  include <sstream>
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
  typename RuntimeDB<T>::LHSRelatedInteractionCtx
  RuntimeDB<T>::findAllLHSRelatedInteractionCtx( const std::string & xpath_extra ) {
    LHSRelatedInteractionCtx retval;

    for (unsigned int A = 0; A < props.size(); ++A) {
      for (unsigned int B = A; B < props.size(); ++B) {
        interaction::Input in(*this, A, B);

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
          "//Interaction/" + getXpathQuery("In", in_eq_set)
          + ( xpath_extra.size() > 0 ? '/' + xpath_extra : "" )
        );
        retval[in] = filter->filter( xml::Context::set( xl.begin(), xl.end() ) );
      }
    }

    /* now filter the interactions to get the desired subset. */
    return retval;
  }


  template < typename T >
  void RuntimeDB<T>::initBinaryInteractions() {
    /* first thing we do is to sort the particle property entries by mass and
     * name. */
    std::sort(props.begin(), props.end(), property::Comparator());

    /* We need to get the set of all particle names to do extra filtering */
    std::string particles_output_filter;
    {
      std::ostringstream istr;
      typedef typename PropertiesVector::iterator PIter;
      for ( PIter i = props.begin(); i != props.end(); ++i ) {
        using property::name;
        istr << ':' << i->name::value << ':';
      }

      particles_output_filter =
        "Eq/Out["
          "T/P[     contains('" + istr.str() + "', concat(':',text(),':'))]"
        "][not("
          "T/P[not( contains('" + istr.str() + "', concat(':',text(),':')))]"
        ")]/../..";
    }

    /* make sure that the side-length of the matrix is set correctly. */
    interactions.resize(props.size());

    /* Get all LHS  related interaction contexts. */
    LHSRelatedInteractionCtx lhs_ctxs =
      findAllLHSRelatedInteractionCtx( particles_output_filter );
    typedef LHSRelatedInteractionCtx::const_iterator LHSCtxIter;

    for (LHSCtxIter lhs_i = lhs_ctxs.begin(); lhs_i != lhs_ctxs.end(); ++lhs_i) {
      interaction::Input const & in = lhs_i->first;

      /* first instantiate the (A,B)th interactions */
      Set & set = interactions( in.A.type, in.B.type );
      set.lhs = in;

      xml::Context::set const & xs = lhs_i->second;
      /* add each of the allowed interactions to the new set. */
      for (xml::Context::set::iterator k = xs.begin(); k != xs.end(); ++k) {
        /* Finally load the Equation fully and push it into the Output stack. */
        set.rhs.push_back(Set::Equation::load(*k,*this));
      }
    }
  }


  template < typename T >
  inline typename RuntimeDB<T>::PropertiesVector::const_iterator
  RuntimeDB<T>::findParticle(const std::string & name) const {
    typedef typename PropertiesVector::const_iterator CIter;
    CIter i = props.begin();
    for (; i != props.end(); i++) {
      property::name n = (*i);
      if (name == n.value)
        break;
    }
    return i;
  }


  template < typename T >
  inline typename RuntimeDB<T>::PropertiesVector::iterator
  RuntimeDB<T>::findParticle(const std::string & name) {
    typedef typename PropertiesVector::iterator Iter;
    Iter i = props.begin();
    for (; i != props.end(); i++) {
      property::name n = (*i);
      if (name == n.value)
        break;
    }
    return i;
  }


  template < typename T >
  inline int RuntimeDB<T>::findParticleIndx(const std::string & name) const {
    typedef typename PropertiesVector::const_iterator CIter;
    CIter i = findParticle(name);
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
  template < typename Iter >
  inline void RuntimeDB<T>::addParticleType(Iter begin, const Iter & end) {
    for ( ; begin != end; ++begin )
      addParticleType( *begin );
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
    typedef typename PropertiesVector::const_iterator CIter;
    CIter i = findParticle(n);
    if (i == props.end())
      throw std::runtime_error("particle type not loaded: '" + n + '\'');
    return  *i;
  }


  template < typename T >
  inline typename RuntimeDB<T>::Properties &
  RuntimeDB<T>::operator[] ( const std::string & n ) {
    typedef typename PropertiesVector::iterator Iter;
    Iter i = findParticle(n);
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


  template < typename T >
  inline const typename RuntimeDB<T>::Set &
  RuntimeDB<T>::operator() ( const std::string & i_name,
                             const std::string & j_name ) const {
    int i = findParticleIndx(i_name);
    int j = findParticleIndx(j_name);
    if (i == -1)
      throw std::runtime_error("particle type not loaded: '" + i_name + '\'');
    if (j == -1)
      throw std::runtime_error("particle type not loaded: '" + j_name + '\'');

    return  interactions(i,j);
  }


  template < typename T >
  inline typename RuntimeDB<T>::Set &
  RuntimeDB<T>::operator() ( const std::string & i_name,
                             const std::string & j_name ) {
    int i = findParticleIndx(i_name);
    int j = findParticleIndx(j_name);
    if (i == -1)
      throw std::runtime_error("particle type not loaded: '" + i_name + '\'');
    if (j == -1)
      throw std::runtime_error("particle type not loaded: '" + j_name + '\'');

    return  interactions(i,j);
  }


  template < typename LHSCtxs >
  inline std::set<std::string> findAllRHSParticles( const LHSCtxs & lhs_ctxs ) {
    std::set<std::string> retval;

    typedef typename LHSCtxs::const_iterator LHSCtxIter;

    for(LHSCtxIter lhs_i = lhs_ctxs.begin(); lhs_i != lhs_ctxs.end(); ++lhs_i) {
      xml::Context::set const & xs = lhs_i->second;
      /* add each of the allowed interactions to the new set. */
      for (xml::Context::set::iterator k = xs.begin(); k != xs.end(); ++k) {
        /* This is a hack until we can figure out how to write an xpath query
         * to make sure that the outputs all come from the particles set.
         * Such an xpath query may end up being impossible. */
        xml::Context::list xl = k->eval("Eq/Out/T/P");
        for (xml::Context::list::iterator i = xl.begin(); i != xl.end(); ++i ) {
          retval.insert( i->parse<std::string>() );
        }
      }
    }

    return retval;
  }


  inline xml::Context::list getAllParticlesCtx( const xml::Doc & xmlDb ) {
    return xmlDb.eval("//Particle");
  }

} /* namespace chimp */

