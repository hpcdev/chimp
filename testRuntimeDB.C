
#include <iostream>
#include <iterator>
#include "ParticleDB.h"
#include "physical_calc.h"
#include <olson-tools/physical/physical.h>
#include <olson-tools/Vector.h>
#include <nsort/Particle.h>
#include <nsort/snort.h>




template <class ParticleIterator>
struct ParticleRangeContainer {
    typedef ParticleIterator particle_iter_type;
    IteratorRange<ParticleIterator> particles;
};

template <class ParticleIterator, class val_map,
          class sub = ParticleRangeContainer<ParticleIterator> >
struct octree_node : ParticleRangeContainer<ParticleIterator> {
    typedef ParticleRangeContainer<ParticleIterator> super;

    void buildOctree() {
        //nsort<type_map> pts(n_types);
        //pts.sort(super::particles.begin(), super::particles.end());
        //type_map::getIterators(pts, static_cast<sub&>(*this));
    }
};

template <class T>
struct upper_half_triangle : std::vector<T> {
    typedef std::vector<T> super;

    upper_half_triangle(const int & n = 0) {
        resize(n);
    }

    void resize(const int & n) {
        this->n = n;
        super::resize(n*(n+1)/2);
    }

    inline int indx(const int & i, const int & j) const {
        int r = j;
        if (i > 0)
            r += indx(i-1,n-1) - i + 1;
        return r;
    }

    T & operator()(const int & i, const int & j) {
        return this->operator[](indx(i,j));
    }

    const T & operator()(const int & i, const int & j) const {
        return this->operator[](indx(i,j));
    }
  private:
    int n;
};

template <class ParticleIterator, class val_map>
struct cell_info :
    octree_node<
        ParticleIterator,val_map,
        cell_info<ParticleIterator,val_map>
    > {

    typedef octree_node<
        ParticleIterator,val_map,
        cell_info<ParticleIterator,val_map>
    > super;

    struct data_t {
        data_t (const double & vav = 0, const double & v2 = 0) : vav(vav), v2(v2) {}
        double vav;
        double v2;
    };

    struct i_info {
        i_info(const double & m = 0) : max_sigma_v_rel(m) {}
        double max_sigma_v_rel;
    };

    cell_info (const int & n =  0) {
        interaction_info.resize(n); /* actually : n*(n+1)/2 */
        types.resize(n);
        data.resize(n);
    }

    void sort_types(const int & n_types) {
        nsort<type_map> pts(n_types);
        pts.sort(super::particles.begin(), super::particles.end());
        type_map::getIterators(pts, (*this));
    }

    /* BEGIN STORAGE MEMBERS */
    std::vector< IteratorRange<ParticleIterator> > types; /* size : n */
    std::vector< data_t > data;                           /* size : n */
    upper_half_triangle<i_info> interaction_info;               /* size : n*(n+1)/2 */
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

    RuntimeDB<> db;
    prepareCalculator(db.xmlDb);

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

    typedef cell_info<Particle::list::iterator,position_3D_map_w_type<0,1,2> > cell_t;
    cell_t cell(db.getProps().size());

    /* initialize single-species velocity data */
    cell.data[db.findParticle("87Rb")]      = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["87Rb"].mass::value));
    cell.data[db.findParticle("Ar"  )]      = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Ar"  ].mass::value));
    cell.data[db.findParticle("e^-" )]      = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["e^-" ].mass::value));
    cell.data[db.findParticle("Hg"  )]      = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Hg"  ].mass::value));
    cell.data[db.findParticle("Hg^+")]      = cell_t::data_t(0, stddev_v_rel(100*uK, 0.5*db["Hg^+"].mass::value));
    /* initialize cross-species data */
    for (unsigned int i = 0; i < cell.data.size(); i++)
        for (unsigned int j = i; j < cell.data.size(); j++) {
            double stddev_v = sqrt(cell.data[i].v2 + cell.data[j].v2);
            /* loop over all possible outputs and obtain the aggregate max. */
            cell.interaction_info(i,j).max_sigma_v_rel =
                db(i,j).find_max_sigma_v_rel_from_stddev_v(stddev_v);
        }


    /* add particles and sort by type */
    cell.particles = IteratorRange<Particle::list::iterator>(particles.begin(), particles.end());
    cell.sort_types(db.getProps().size());
    std::cout << "\nsorted by nsort(type):\n";
    std::copy(cell.particles.begin(), cell.particles.end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    std::cout << "\ttype(2):\n";
    std::copy(cell.types[2].begin(), cell.types[2].end(), std::ostream_iterator<Particle>(std::cout, "\n"));
    std::cout << std::endl;


    /* ***** END MAKE PARTICLES AND CELL ***** */


    /* spit out short info on all known particle types */
    for (unsigned int i = 0; i < db.getProps().size(); i++)
        std::cout << db[i] << std::endl;

    /* spit out known interactions and attempt execution */
    for (unsigned int i = 0; i < db.getProps().size(); i++)
        for (unsigned int j = i; j < db.getProps().size(); j++) {
            using particledb::interaction::Set;
            using physical::unit::nm;
            using olson_tools::SQR;
            Set & set = db(i,j);

            if (set.rhs.size() == 0)
                continue;

            set.print(std::cout, db) << '\n';

            /* now let's try and pick a mock collision pair for this
             * interaction. */
            if (cell.types[i].size() == 0 || cell.types[j].size() == 0)
                continue;
            Particle::list::iterator pi =
                cell.types[i].begin() + int(cell.types[i].size()*MTRNGrand()*0.99999999);
            Particle::list::iterator pj =
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

    std::cout << std::endl;

    return 0;
}
