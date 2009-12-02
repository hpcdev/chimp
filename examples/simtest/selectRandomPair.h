
namespace simtest {

  template < typename RandomAccessParticleContainer >
  std::pair<
    typename RandomAccessParticleContainer::iterator,
    typename RandomAccessParticleContainer::iterator
  >
  selectRandomPair( RandomAccessParticleContainer & Aparticles,
                    RandomAccessParticleContainer & Bparticles ) {
    /* Pick the particles that need to interact. */
    Particle::list::iterator pi
         = Aparticles.begin() + int(Aparticles.size()*MTRNGrand()*0.99999999);

    Particle::list::iterator pj = pi;

    while ( pi == pj )
      pj = BParticles.begin() + int(BParticles.size()*MTRNGrand()*0.99999999);

    return std::make_pair(pi, pj);
  }

}

