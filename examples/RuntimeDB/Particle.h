
#ifndef PARTICLEDB_EXAMPLE_PARTICLE_H
#define PARTICLEDB_EXAMPLE_PARTICLE_H

#include <vector>
#include <olson-tools/Vector.h>
#include <cstdlib>


struct Particle {
    typedef std::vector<Particle> list;

    Particle(const olson_tools::Vector<double,3> & x = 0.0,
             const olson_tools::Vector<double,3> & v = 0.0,
             const int & type = 0) : x(x), v(v), type(type) {}
    olson_tools::Vector<double, 3> x;
    olson_tools::Vector<double, 3> v;
    int type;
};


std::ostream & operator<< (std::ostream & out, const Particle & p) {
    return out << "{x: (" << p.x[0] << ", " << p.x[1] << ", " << p.x[2] << "), "
                   "v: (" << p.v[0] << ", " << p.v[1] << ", " << p.v[2] << "), "
                   "t: " << p.type   << '}';
}

void initPVector(Particle::list & pv, const int & n, const int & n_type) {
    static const unsigned int SEED = time(NULL);
    using olson_tools::Vector;
    srand(SEED);
    pv.resize(n);
    for (int i = 0; i < n; i++) {
        Vector<double,3> x = V3(
                            100.0*rand()/((double)RAND_MAX) - 50.0,
                            100.0*rand()/((double)RAND_MAX) - 50.0,
                            100.0*rand()/((double)RAND_MAX) - 50.0
                         ),
                         v = V3(
                            100.0*rand()/((double)RAND_MAX) - 50.0,
                            100.0*rand()/((double)RAND_MAX) - 50.0,
                            100.0*rand()/((double)RAND_MAX) - 50.0
                         );
        int type = (int)rint( ((double)(n_type-1)*rand())/((double)RAND_MAX) );
        pv[i] = Particle(x,v,type);
    }
}

void initPtrVector( Particle::list  & pv,
                    std::vector<Particle*> & ptrv,
                    const int & n, const int & n_type) {
    /* with ptr array */
    initPVector(pv, n, n_type);
    ptrv.resize(n);
    for (unsigned int i = 0; i < pv.size(); ++i)
        ptrv[i] = & pv[i];
}

#endif // PARTICLEDB_EXAMPLE_PARTICLE_H
