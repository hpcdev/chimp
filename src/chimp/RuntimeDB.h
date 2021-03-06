/*==============================================================================
 * Public Domain Contributions 2009 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Copyright (C) 2006, 2008 Spencer E. Olson                                   *
 *                                                                             *
 * This file is part of CHIMP                                                  *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


// -*- c++ -*-
// $Id: Particle.h,v 1.3 2005/06/07 10:00:14 olsonse Exp $

/** \file RuntimeDB.h
 * A generic particle database to provide data for collisions and such.  
 *
 * Copyright 2000-2008 Spencer Olson
 *
 * As this evolves, this will most likely need to change to storing data for
 * particles in a hard file (not compiled in) format.  Thus, this database
 * will perhaps inherit some of the functionality of my old Materials database
 * from XUVMulti.  This allows the database to cache values after reading them
 * from file once, and also provides for interpolating between values, in this
 * case, interpolating cross sections between energy values.  
 *
 * TODO:
 *     Add functionality for adding new types of particles (runtime).  This
 *     could be accomplished be allowing new full definitions to be specified
 *     as well as allowing pre-existing particle types to be duplicated.  This
 *     might come in handy if we decide to keep some integer types of particle
 *     data out of the Particle class, but left in the particle_type database.
 *     Integer items could be charge (in units of q_e), internal atomic state
 *     (internal quantum numbers), and so on.  To try and make this at least
 *     partially efficient and easy to use for such 'sub-types', we might
 *     create actual sub-particle types that would be easy to access via
 *     integer arithmetic.  This might make it easier for instance to change
 *     the internal atomic state or even the particle charge without doing any
 *     work with "getParticleTypeIndex(...)" or the like.  It would be most
 *     easy to just _know_ what the particle species index needs to be based on
 *     the knowledge of how the charge/internal quantum number has changed.  
 *
 *     I really need two types of classes: one class will act as the data
 *     lookup and storage class, reading from some extensible file format like
 *     xml, and the other will be a class of actual used data items, like for
 *     instance mass, size, charge, (cross-section, energy list).  
 *     The type of information would either constitute a "model" or rather
 *     include a "model" such as a "collision model"  (this would have the
 *     information necessary for the particular model, such as VSS, VHS, ...).
 *
 *     For right now, I'm just going to keep my dual-functionality class until
 *     I work out the details a little more.  
 *
 *     Another Requirement:
 *     It is necessary that the "species" index be an integer, preferably of low
 *     value.  In other words, if only 5 particle types are loaded from file,
 *     ONLY 5 values of "species" should be used, i.e. '0, 1, 2, 3, 4'.  Some
 *     classes, such as dsmc::Wall may use this to create an array of items
 *     that are used only with a particular species.  
 *
 *     Another requirement:
 *     I believe that it is necessary to have all pathways between particle
 *     types (i.e. quantum trajectories, dissociation, association, etc.)
 *     should be set up during the initialization of the runtime database (at
 *     time of xml-file reading).
 *     Perhaps there need to be a set of data in the xml file that defines the
 *     possible pathways that can exist.  
 *     The particular simulation/collision model will then have to read the
 *     pathways that it is interested in.  
 *     I am currently of the belief that the pathways are best implemented by
 *     a linked list, such that from one particle, you can only move over one
 *     link at a time.  (This does not mean that it is not possible to have
 *     multiple links).  
 *
 *     Another requirement:
 *     We actually need two types of runtime databases:
 *     1.  A particle properties database that contains data such as mass,
 *     diameter, charge.
 *     2.  An interaction properties database that contains data needed for
 *     all allowed interactions such as cross-species collision cross-sections
 *     (or just the cross-section for a mono-species interaction), reduced
 *     mass, and so on.  This set of data will be used by the DSMC class to
 *     perform collisions for instances.
 *     This second database would likely be best implemented by a 2D array
 *     that just represents the Cartesian join of the single-particle dataset
 *     (except that the entries would be averaged appropriately).   
 *
 * @see ParticleDB.C for notes regarding changes. 
 *
 */

#ifndef chimp_RuntimeDB_h
#define chimp_RuntimeDB_h

#  include <chimp/default_data.h>
#  include <chimp/make_options.h>
#  include <chimp/interaction/Set.h>
#  include <chimp/interaction/model/Base.h>
#  include <chimp/interaction/cross_section/Base.h>
#  include <chimp/interaction/filter/Base.h>
#  include <chimp/property/name.h>
#  include <chimp/property/Comparator.h>

#  include <xylose/XSTR.h>
#  include <xylose/logger.h>
#  include <xylose/xml/Doc.h>
#  include <xylose/compat/math.hpp>
#  include <xylose/upper_triangle.h>

#  include <ostream>
#  include <fstream>
#  include <cfloat>
#  include <set>
#  include <string>
#  include <vector>
#  include <algorithm>



namespace chimp {

  using boost::shared_ptr;
  namespace xml = xylose::xml;


  /** Runtime database of properties pertinent to the current simulation.
   * @param _options
   *    Policy configuration changes the behavior of the RuntimeDB class.  This
   *    class sets, for instance, the set of particle properties that are loaded
   *    from the xml dataset.  <br>
   *    [ Default:  make_options<>::type ]
   *
   * @see make_options
   */
  template < typename _options = make_options<>::type >
  class RuntimeDB {
    /* TYPEDEFS */
  public:
    /** The options of the RuntimeDB class.  This is a typedef copy of the
     * _options template parameter. */
    typedef _options options;

    /** Particle Properties that are loaded from the xml file. */
    typedef typename options::Properties Properties;

    /** Set of interactions equations that share the same inputs. */
    typedef interaction::Set<options> Set;

    /** Cross section Base type. */
    typedef interaction::cross_section::Base<options> CrossSection;

    /** Interaction model Base type. */
    typedef interaction::model::Base<options> Interaction;

    /** Map of cross section "name" to model. */
    typedef std::map<
      std::string,
      shared_ptr< CrossSection >
    > CrossSectionRegistry;

    /** Map of interaction "name" to model. */
    typedef std::map<
      std::string,
      shared_ptr< Interaction >
    > InteractionRegistry;

    /** Data type for the Interaction table.  This is really just a wrapper
     * around the std::vector class such that using operator()(i,j) works easily
     * and correctly. */
    typedef xylose::upper_triangle<
      Set,
      xylose::SymmetryFix
    > InteractionTable;

    /** Vector type used to store all loaded particle properties. */
    typedef std::vector<Properties> PropertiesVector;

    /** Map of interaction Input to xml::Context::set instances for all
     * interactions that match the input.
     * @see findAllLHSRelatedInteractionCtx
     */
    typedef std::map<
      interaction::Input,
      xml::Context::set
    > LHSRelatedInteractionCtx;



    /* MEMBER STORAGE */
  public:
    /** XML document from which data is extracted.  */
    xml::Doc xmlDb;

    /** Registry for cross section functor classes. */
    CrossSectionRegistry cross_section_registry;

    /** Registry for interaction functor classes. */
    InteractionRegistry interaction_registry;

    /** Interaction Equation filter.  This filter is used to determine the
     * interactions that are allowed in each cell of the interaction table.
     * This filter will be executed separately for each cell in the table and
     * hence will have an implicit Input filter based on the input elements for
     * the interaction table cell.  The default filter will allow ONLY elastic
     * collisions. */
    shared_ptr<interaction::filter::Base> filter;

    /** Specifies the maximum range overwhich to sum the diameters of the
     * auto-combined non vhs-vhs cross section pairs.  Extrapolation beyond this
     * range will follow the normal procedure.  IF <=0, only vhs-vhs cross
     * sections can be combined.  
     * [Default: 0]
     */
    double default_ElasticCreator_vmax;

    /** Specifies the resolution withwhich to sum the diameters of the
     * auto-combined non vhs-vhs cross section pairs.  IF <=0, dv will be chosen
     * to give 100 data points.  
     * [Default: 0]
     */
    double default_ElasticCreator_dv;

  private:
    /** Vector of particle properties.
     * Note that the order of the entries in the properties vector is NOT well
     * determined, until AFTER initBinaryInteractions() has been called.  */
    PropertiesVector props;

    /** Initialized at time of initBinaryInteractions() call. */
    InteractionTable interactions;





    /* MEMBER FUNCTIONS */
  public:
    /** Constructor opens up the PARTICLE_XML xml file, prepares the units
     * calculator and registers the default models.
     */
    RuntimeDB( const std::string & xml_doc = default_data::particledb() );

    /** Add XML section data into the already loaded CHIMP XML data set. */
    inline void addXMLData( const std::string & filename );

    /** Loads the particle information for the given particle name into the
     * runtime database.  Note that only the information relevant to the
     * templated Properties class will get loaded. 
     *
     * initBinaryInteractions() should be called AFTER this.
     * */
    inline void addParticleType(const std::string & name);

    /** Loads the particle information for the each of the given particles
     * from the iterator range into the runtime database.  Note that only the
     * information relevant to the templated Properties class will get loaded. 
     *
     * initBinaryInteractions() should be called AFTER this.
     * */
    template < typename Iter >
    inline void addParticleType(Iter begin, const Iter & end);

    /** Loads the particle information from the given xml-context.
     * Note that only the information relevant to the templated Properties
     * class will get loaded. 
     *
     * @see addParticleType(const std::string & name)
     * */
    inline void addParticleType(const xml::Context & x);

    /** Adds an already loaded Properties class into the particle properties
     * array only if it doesn't already exist. */
    inline void addParticleType(const Properties & prop);

    /** Adds particles and specifies an interaction filter from a model. */
    inline void addModel( const std::string & model_name );


    /** Read-only access to the properties vector.
     *
     * NOTE:  The order of the entries in the properties vector is <b>NOT</b>
     * well determined, until <b>AFTER</b> initBinaryInteractions() has been
     * called.
     */
    const PropertiesVector & getProps() const { return props; }

    /** Read-only access to the interactions matrix. */
    const InteractionTable & getInteractions() const { return interactions; }

    /** return the set of single-species properties for the given species.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     * */
    const Properties & operator[](const int & i) const { return props[i]; }
    /** return the set of single-species properties for the given species.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     * */
          Properties & operator[](const int & i)       { return props[i]; }

    /** return the set of single-species properties for the given species. */
    inline const Properties & operator[](const std::string & n) const;

    /** return the set of single-species properties for the given species. */
    inline       Properties & operator[](const std::string & n);

    /** return the set of cross-species properties for the two given species. */
    inline const Set & operator()(const int & i, const int & j) const;

    /** return the set of cross-species properties for the two given species. */
    inline       Set & operator()(const int & i, const int & j);

    /** return the set of cross-species properties for the two given species. */
    inline
    const  Set & operator()(const std::string & i, const std::string & j) const;

    /** return the set of cross-species properties for the two given species. */
    inline Set & operator()(const std::string & i, const std::string & j);


    /** Get the (const) iterator of the particle species with the specified name.
     * @return Iterator of particle species or getProps().end() if not found.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    inline typename PropertiesVector::const_iterator
    findParticle(const std::string & name) const;

    /** Get the (non-const) iterator of the particle species with the specified name.
     * @return Iterator of particle species or getProps().end() if not found.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    inline typename PropertiesVector::iterator
    findParticle(const std::string & name);

    /** Get the index of the particle species with the specified name.
     * @return Index of particle species or -1 if not found.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    inline int findParticleIndx(const std::string & name) const;

    /** Set up the table for interactions with binary inputs. */
    void initBinaryInteractions();

    /** Create the set of all interaction equations that match the left hand
     * side given the current set of load particles.
     *
     * @param xpath_extra
     *    An optional xpath query to limit the equations that are returned.
     *    This optional xapth predicate begins at the Interaction context and
     *    must return back to the Interaction context.
     */
    LHSRelatedInteractionCtx
    findAllLHSRelatedInteractionCtx( const std::string & xpath_extra = "" );

    /** Add in missing elastic cross-species cross sections, assuming that the
     * single-species cross section exists and is already loaded.
     *
     * @param i
     *   Only create the missing elastic cross sections for ith species.  This
     *   defaults to match all species.
     * @param j
     *   Only create the missing elastic cross sections for jth species.  This
     *   defaults to match all species.
     * @param vmax
     *   Range overwhich to sum non vhs-vhs cross-section pairs.
     *   @see default_ElasticCreator_vmax
     * @param dv
     *   Resolution withwhich to sum non vhs-vhs cross-section pairs.
     *   @see default_ElasticCreator_dv
     *
     * @return Number of elastic cross sections created.
     *
     * @see createMissingElasticCrossSections( const std::string &, const std::string & ).
     */
    int createMissingElasticCrossSections( const int & i = -1,
                                           const int & j = -1,
                                           double vmax = 0.0,
                                           double dv = 0.0 );

    /** Add in missing elastic cross-species cross sections, assuming that the
     * single-species cross section exists and is already loaded.
     *
     * @param i
     *   Only create the missing elastic cross sections for ith named species.
     *   This defaults to match all species.
     * @param j
     *   Only create the missing elastic cross sections for jth named species.
     *   This defaults to match all species.
     *
     * @return Number of elastic cross sections created.
     *
     * @see createMissingElasticCrossSections( const int &,
     *                                         const int &,
     *                                         double, double ).
     */
    inline
    int createMissingElasticCrossSections( const std::string & i,
                                           const std::string & j = "",
                                           const double & vmax = 0.0,
                                           const double & dv = 0.0 );

  };/* RuntimeDB */


  /** Create a set of all particles that result from interactions that relate to
   * the given left-hand-side portions of the equation.
   *
   * @return A set of all particle names resulting from equations in the form of
   * a <code>std::set< std::string ></code>.
   *
   * @see findAllLHSRelatedInteractionCtx for obtaining a map of
   * interaction::Input to a list of interactions xml::Context nodes comprising
   * an xml::Context::set.
   */
  template < typename LHSCtxs >
  inline std::set<std::string> findAllRHSParticles( const LHSCtxs & lhs_ctxs );

  /** Create a list of xml::Context intances for each particle in the xml
   * database. */
  inline xml::Context::list getAllParticlesCtx( const xml::Doc & xmlDb );

} /* namespace chimp */

#include <chimp/RuntimeDB.cpp>

#endif // chimp_RuntimeDB_h
