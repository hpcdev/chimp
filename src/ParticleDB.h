// -*- c++ -*-
// $Id: Particle.h,v 1.3 2005/06/07 10:00:14 olsonse Exp $

/** \file A generic particle database to provide data for collisions and such.  
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
 *     easy to just _know_ what the particle type index needs to be based on
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
 *     It is necessary that the "type" index be an integer, preferably of low
 *     value.  In other words, if only 5 particle types are loaded from file,
 *     ONLY 5 values of "type" should be used, i.e. '0, 1, 2, 3, 4'.  Some
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

#ifndef PARTICLEDB_PARTICLEDB_H
#define PARTICLEDB_PARTICLEDB_H

#  include <math.h>
#  include <ostream>
#  include <fstream>
#  include <cfloat>
#  include <set>
#  include <string>
#  include <algorithm>


#  include <olson-tools/logger.h>
#  include <olson-tools/upper_triangle.h>
#  include <olson-tools/xml/XMLDoc.h>


#  include "interaction/Interaction.h"
#  include "interaction/Set.h"
#  include "Particle.h"

namespace particledb {

    namespace xml = olson_tools::xml;



/** Runtime database of properties pertinent to the current simulation. */
template <class _Properties = Particle::Properties>
class RuntimeDB {
  public:
    typedef _Properties prop_type;
    typedef std::vector<prop_type> prop_list;
    typedef olson_tools::upper_triangle<interaction::Set, olson_tools::SymmetryFix> InteractionMatrix;

  private:
    /** Vector of particle properties.
     * Note that the order of the entries in the properties vector is NOT well
     * determined, until AFTER initInteractions() has been called.  */
    prop_list props;

    /** Initialized at time of initInteractions() call. */
    InteractionMatrix interactions;

  public:
    /** Set of interactions to allow.
     * This set should be built before calling initInteractions.  When
     * initInteractions is called, it will consult this set to filter out
     * any equations that are not to be allowed.
     * */
    std::set<std::string> allowed_equations;

    xml::XMLDoc xmlDb;

    RuntimeDB(const std::string & xml_doc = "particledb.xml") : xmlDb(xml_doc) {}

    /** Read-only access to the properties vector.
     *
     * NOTE:  The order of the entries in the properties vector is <b>NOT</b>
     * well determined, until <b>AFTER</b> initInteractions() has been called.
     * */
    const prop_list & getProps() const { return props; }

    /** Read-only access to the interactions matrix. */
    const InteractionMatrix & getInteractions() const { return interactions; }

    /** return the set of single-species properties for the given species.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     * */
    const prop_type & operator[](const int & i) const { return props[i]; }
    /** return the set of single-species properties for the given species.
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     * */
          prop_type & operator[](const int & i)       { return props[i]; }

    /** return the set of single-species properties for the given species. */
    const prop_type & operator[](const std::string & n) const {
        typename prop_list::const_iterator i = findParticle(n);
        if (i == props.end())
            throw std::runtime_error("particle type not loaded: '" + n + '\'');
        return  *i;
    }
    /** return the set of single-species properties for the given species. */
          prop_type & operator[](const std::string & n)       {
        typename prop_list::iterator i = findParticle(n);
        if (i == props.end())
            throw std::runtime_error("particle type not loaded: '" + n + '\'');
        return  *i;
    }

    /** return the set of cross-species properties for the two given species. */
    const interaction::Set & operator()(const int & i, const int & j) const {
        return interactions(i,j);
    }
    /** return the set of cross-species properties for the two given species. */
          interaction::Set & operator()(const int & i, const int & j)       {
        return interactions(i,j);
    }

    /** Get the (const) iterator of the particle type with the specified name.
     * @return Iterator of particle type or getProps().end() if not found.
     * @see Particle::type and ParticleTypeInfo
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    typename prop_list::const_iterator findParticle(const std::string & name) const {
        typename prop_list::const_iterator i = props.begin();
        for (; i != props.end(); i++) {
            Particle::property::name n = (*i);
            if (name == n.value)
                break;
        }
        return i;
    }

    /** Get the (non-const) iterator of the particle type with the specified name.
     * @return Iterator of particle type or getProps().end() if not found.
     * @see Particle::type and ParticleTypeInfo
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    typename prop_list::iterator findParticle(const std::string & name) {
        typename prop_list::iterator i = props.begin();
        for (; i != props.end(); i++) {
            Particle::property::name n = (*i);
            if (name == n.value)
                break;
        }
        return i;
    }

    /** Get the index of the particle type with the specified name.
     * @return Index of particle type or -1 if not found.
     * @see Particle::type and ParticleTypeInfo
     * @see Note for getProps() concerning ill-determined order of properties
     * vector.
     */
    int findParticleIndx(const std::string & name) const {
        typename prop_list::const_iterator i = findParticle(name);
        if (i == props.end())
            return -1;
        return i - props.begin();
    }

    /** Loads the particle information for the given particle name into the
     * runtime database.  Note that only the information relevant to the
     * templated prop_type class will get loaded. 
     *
     * initInteractions() should be called AFTER this.
     * */
    void addParticleType(const std::string & name) {
        xml::XMLContext x = xmlDb.root_context.find("//Particle[@name=\"" + name + "\"]");
        addParticleType(x);
    }

    /** Loads the particle information from the given xml-context.
     * Note that only the information relevant to the templated prop_type
     * class will get loaded. 
     *
     * @see addParticleType(const std::string & name)
     * */
    void addParticleType(const xml::XMLContext & x) {
        prop_type prop = prop_type::load(x);
        addParticleType(prop);
    }

    void addParticleType(const prop_type & prop) {
        using Particle::property::name;
        const std::string & n = prop.name::value;

        /* See if this particle type has already been
         * loaded into the database (we don't want any duplicates). */
        if (findParticle(n) != props.end())
            olson_tools::logger::log_warning("particle type '%s' was previously loaded; will not reload", n.c_str());
        else
            props.push_back(prop);
    }

    struct props_comparator {
        bool operator()(const _Properties & lhs, const _Properties & rhs) {
            using Particle::property::mass;
            return lhs.mass::value < rhs.mass::value;
        }
    };

    void initInteractions() {
        using interaction::Input;
        using interaction::Equation;
        using interaction::Set;
        using interaction::find_all_interactions;;
        using interaction::find_elastic_interactions;;
        using interaction::filter_interactions;;
        using xml::XMLContext;

        /* first thing we do is to sort the particle property entries by mass. */
        std::sort(props.begin(), props.end(), props_comparator());

        interactions.resize(props.size());

        for (unsigned int A = 0; A < props.size(); ++A) {
            for (unsigned int B = A; B < props.size(); ++B) {
                using std::string;
                using Particle::property::name;
                const string & n_A = props[A].name::value;
                const string & n_B = props[B].name::value;

                /* get a set of ALL interactions. */
                XMLContext::set xl = find_all_interactions(xmlDb.root_context, n_A, n_B);
                /* filter by set of allowed equations. */
                xl = filter_interactions(xl, allowed_equations);
                /* Add in elastic interactions. */
                XMLContext::set xe = find_elastic_interactions(xmlDb.root_context, n_A, n_B);
                xl.insert(xe.begin(), xe.end());


                /* first instantiate the (A,B)th interactions */
                Set & set = interactions(A,B);
                set.lhs.setInput(*this,A,B);

                /* add each of the allowed interactions to the new set. */
                for (XMLContext::set::iterator k = xl.begin(); k != xl.end(); k++)
                    set.rhs.push_back(Equation::load(*k,*this));
            }
        }
    }

    /** Creates and returns a static instance of the RuntimeDB.  Note that
     * only during the first time this is called will the database become
     * instantiated/initialized.  Consecutive calls will simply return the
     * reference to the already-instantiated class.  */
    static RuntimeDB & instance() {
        static RuntimeDB * db = new RuntimeDB;
        return *db;
    }

};

} /* namespace particledb */

#endif // DSMC_PARTICLEDB_H
