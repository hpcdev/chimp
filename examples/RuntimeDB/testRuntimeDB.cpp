
#include "Particle.h"

#include <particledb/ParticleDB.h>
#include <particledb/physical_calc.h>

#include <olson-tools/upper_triangle.h>
#include <olson-tools/physical/physical.h>
#include <olson-tools/Vector.h>

#include <olson-tools/nsort.h>
#include <olson-tools/nsort_maps.h>


#include <iostream>
#include <iterator>


using olson_tools::upper_triangle;
using olson_tools::IteratorRange;
using olson_tools::nsort;
using olson_tools::type_map;


#ifndef   XML_FILENAME
#  define XML_FILENAME  "particledb.xml"
#endif


template <class ParticleIterator>
struct cell_info {
    typedef ParticleIterator particle_iter_type;

    struct data_t {
        data_t (const double & vav = 0, const double & v2 = 0) : vav(vav), v2(v2) {}
        double vav;
        double v2;
    };

    struct interact_info {
        interact_info(const double & m = 0) : max_sigma_v_rel(m) {}
        double max_sigma_v_rel;
    };


    cell_info (const int & n =  0) {
        interaction_info.resize(n); /* actually : n*(n+1)/2 */
        types.resize(n);
        data.resize(n);
    }

    void sort_types(const int & n_types) {
        nsort<type_map> pts(n_types);
        pts.sort(particles.begin(), particles.end());
        type_map::getIterators(pts, (*this));
    }

    /* BEGIN STORAGE MEMBERS */
    IteratorRange<ParticleIterator> particles;
    std::vector< IteratorRange<ParticleIterator> > types; /* size : n */
    std::vector< data_t > data;                           /* size : n */
    upper_triangle<interact_info> interaction_info;  /* size : n*(n+1)/2 */
    /* END STORAGE MEMBERS */
};





template <class IteratorRangeContainer, class InteractionInfoVector>
void doCollisions( const IteratorRangeContainer & iv, InteractionInfoVector & intv ) {
    typedef typename IteratorRangeContainer::value_type IteratorRange;
    typedef typename IteratorRange::iter_type Iter;
}

int main() {
    const int N_part = 10;

    using particledb::RuntimeDB;
    using particledb::interaction::stddev_v_rel;
    using physical::unit::m;
    using physical::unit::s;
    using physical::unit::uK;

    RuntimeDB<> db(XML_FILENAME);
    particledb::prepareCalculator(db.xmlDb);

    /* load information from XML file. */
    db.addParticleType("87Rb");
    db.addParticleType("Ar"  );
    db.addParticleType("e^-" );
    db.addParticleType("Hg"  );
    db.addParticleType("Hg^+");

    db.allowed_equations.insert("(Ar)+(87Rb)->(Ar)+(87Rb)");
    db.allowed_equations.insert("(e^-)+(Hg)->(e^-)+(Hg^+)");

    /* set up the the runtime database */
    db.initInteractions();
    /* close the xml-file and free associated resources. */
    db.xmlDb.close();


    /* ***** BEGIN MAKE PARTICLES AND CELL ***** */
    Particle::list particles;
    initPVector(particles, N_part, db.getProps().size());

    typedef cell_info<Particle::list::iterator> cell_t;
    cell_t cell(db.getProps().size());

    /* initialize single-species velocity data */
    cell.data[db.findParticleIndx("87Rb")] = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["87Rb"].mass::value));
    cell.data[db.findParticleIndx("Ar"  )] = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Ar"  ].mass::value));
    cell.data[db.findParticleIndx("e^-" )] = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["e^-" ].mass::value));
    cell.data[db.findParticleIndx("Hg"  )] = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Hg"  ].mass::value));
    cell.data[db.findParticleIndx("Hg^+")] = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Hg^+"].mass::value));

    /* initialize cross-species data */
#if 0
    /* depending on your point of view, this method of moving through the
     * interaction items might be easier to read, but it should prove to be
     * slower than the method below. */
    for (unsigned int i = 0; i < cell.data.size(); i++)
        for (unsigned int j = i; j < cell.data.size(); j++) {
            double stddev_v = sqrt(cell.data[i].v2 + cell.data[j].v2);
            /* loop over all possible outputs and obtain the aggregate max. */
            cell.interaction_info(i,j).max_sigma_v_rel =
                db(i,j).find_max_sigma_v_rel_from_stddev_v(stddev_v);
            if (&db(i,j) != &db(j,i))
                std::cout << "************** DIFFERENCES ********** " << std::endl;
        }

#else
    {
        /* This method of moving through the interaction items should prove to
         * faster than the method where we index the items at each reference.
         * */
        using particledb::interaction::Set;
        typedef upper_triangle<cell_t::interact_info>::iterator iIter;
        typedef upper_triangle<Set>::const_iterator sIter;
        iIter ii = cell.interaction_info.begin();
        sIter di = db.getInteractions().begin();
        for (; ii != cell.interaction_info.end(); ++ii, ++di) {
            double stddev_v = sqrt(  cell.data[di->lhs.A].v2
                                   + cell.data[di->lhs.B].v2
                              );
            /* loop over all possible outputs and obtain the aggregate max. */
            ii->max_sigma_v_rel = di->find_max_sigma_v_rel_from_stddev_v(stddev_v);
        }
    }
#endif


    /* add particles and sort by type */
    cell.particles = IteratorRange<Particle::list::iterator>(particles.begin(), particles.end());
    cell.sort_types(db.getProps().size());
    std::cout << "\nsorted by nsort(type):\n";
    std::copy(cell.particles.begin(), cell.particles.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    std::cout << "\tall particles of type(2):\n";
    std::copy(cell.types[2].begin(), cell.types[2].end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    /* ***** END MAKE PARTICLES AND CELL ***** */


    /* spit out short info on all known particle types */
    std::cout << "All particles: " << '\n';
    for (unsigned int i = 0; i < db.getProps().size(); i++)
        std::cout << i << ":  " << db[i] << std::endl;

    std::cout << "All Interaction Inputs: " << '\n';
    std::cout << '\t';
    for (unsigned int i = 0; i < db.getProps().size(); i++)
        std::cout << "      " << db[i].name::value << "\t         ";
    /* spit out table of interactions inputs */
    for (unsigned int i = 0; i < db.getProps().size(); i++) {
        std::cout << '\n' << db[i].name::value << '\t';
        for (unsigned int j = 0; j < db.getProps().size(); j++) {
            using particledb::interaction::Set;
            Set & set = db(i,j);
            set.lhs.print(std::cout << '{' << i << ':' << set.lhs.A << ',' << j << ':' << set.lhs.B << "} ", db) << '\t';
        }
    }



    /* spit out known interactions and attempt execution */
    std::cout << "\n\nAll known interactions and a small calculateOutPath test:\n";
#if 1
    {
    using particledb::interaction::Set;
    typedef upper_triangle<Set>::const_iterator sIter;
    sIter di = db.getInteractions().begin();
    for (; di != db.getInteractions().end(); ++di) {
        using physical::unit::nm;
        using olson_tools::SQR;
        const Set & set = (*di);
        const int & A = set.lhs.A;
        const int & B = set.lhs.B;

        if (set.rhs.size() == 0)
            continue;

        set.print(std::cout, db) << '\n';

        /* now let's try and pick a mock collision pair for this
         * interaction. */
        if ((A == B && cell.types[A].size() < 2) ||
            (cell.types[A].size() == 0 || cell.types[B].size() == 0))
            continue;

        Particle::list::iterator pi =
            cell.types[A].begin() + int(cell.types[A].size()*MTRNGrand()*0.99999999);
        Particle::list::iterator pj = pi;
        while(pi == pj)
            pj =
            cell.types[B].begin() + int(cell.types[B].size()*MTRNGrand()*0.99999999);

        double v_rel = (pi->v - pj->v).abs();

        double & m_s_v = cell.interaction_info(A,B).max_sigma_v_rel;
        std::pair<int,double> path = set.calculateOutPath(m_s_v, v_rel);
        if (path.first < 0)
            std::cout << "\tno out path\n";
        else
            set.rhs[path.first].print(
                std::cout << "-----\tinputs = {" << (*pi) << ',' << (*pj) << "}\n"
                          << "-----\tmax_sigma_v  :  " << m_s_v << "\n"
                          << "-----\tv_rel        :  " << v_rel << "\n"
                          << "-----\tout path     :  ", db) << "\n"
                             "-----\tcross section:  " << (path.second/SQR(nm)) << " nm^2\n";
    }
    }
#elif 1
    for (unsigned int i = 0; i < db.getProps().size(); i++)
        for (unsigned int j = i; j < db.getProps().size(); j++) {
            using particledb::interaction::Set;
            using physical::unit::nm;
            using olson_tools::SQR;
            Set & set = db(i,j);

            if (set.rhs.size() == 0)
                continue;

            set.print(std::cout << '{' << i << ':' << set.lhs.A << ',' << j << ':' << set.lhs.B << '}', db) << '\n';

            /* now let's try and pick a mock collision pair for this
             * interaction. */
            if ((i == j && cell.types[i].size() < 2) ||
                (cell.types[i].size() == 0 || cell.types[j].size() == 0))
                continue;

            Particle::list::iterator pi =
                cell.types[i].begin() + int(cell.types[i].size()*MTRNGrand()*0.99999999);
            Particle::list::iterator pj = pi;
            while(pi == pj)
                pj =
                cell.types[j].begin() + int(cell.types[j].size()*MTRNGrand()*0.99999999);

            double v_rel = (pi->v - pj->v).abs();

            double & m_s_v = cell.interaction_info(i,j).max_sigma_v_rel;
            std::pair<int,double> path = set.calculateOutPath(m_s_v, v_rel);
            if (path.first < 0)
                std::cout << "\tno out path\n";
            else
                set.rhs[path.first].print(
                    std::cout << "-----\tinputs = {" << (*pi) << ',' << (*pj) << "}\n"
                              << "-----\tmax_sigma_v  :  " << m_s_v << "\n"
                              << "-----\tv_rel        :  " << v_rel << "\n"
                              << "-----\tout path     :  ", db) << "\n"
                                 "-----\tcross section:  " << (path.second/SQR(nm)) << " nm^2\n";
        }
#endif

    std::cout << std::endl;

    return 0;
}
