#ifndef chimplib_examples_simtest_opDSMC_h
#define chimplib_examples_simtest_opDSMC_h

#include <chimp/RuntimeDB.h>
#include <chimp/interaction/ParticleAccessors.h>

namespace chimp {
  namespace interaction {

    /** Unity transform of integer value.  The compiler should be able to
     * optimize this class away. */
    struct UnityTransform {
      const int & operator[] ( const int & i ) const { return i; }
    };


    /** CollisionsDriver class for performing all interactions necessary for all the types
     * that are present.
     *
     * @tparam SpeciesMap
     *    Class used to specify which Range iteration corresponds to which
     *    species as specified in the database.  Typically, a calling class
     *    might just actually have all Range instances for each species defined
     *    in the database, but it is also possible that a calling class might
     *    only want to have Range instances for the species of particles that
     *    fit within the current scope (in a particular cell for example).  In
     *    this case, the calling code can create a class that supports the
     *    operator[] function (std::vector for example) that will return the
     *    Range index that corresponds to the database-known species.  A value
     *    of -1 will indicate that no Range exists for that species.  <br>
     *    [Default:  UnityTransform].
     *
     *    FIXME:  Do I really want to do this?  It really makes more sense to do
     *    this if I were to use a double for loop to iterate over all the
     *    particle type-pairs, rather than iterating over all the interactions
     *    as is currently done.  In other words, it makes more sense to have the
     *    map be "Range index to species index", rather than "species index to
     *    range index."  This way, I don't need the special -1 value and the map
     *    only needs to be as long as the number of Range instances that the
     *    calling class provides.  
     */
    template < some template parameters to specialize this for specific use,
               typename SpeciesMap = UnityTransform >
    struct CollisionsDriver {

      template < typename SpeciesIterator,
                 typename InteractionInfo,
                 typename options,
                 typename BackInsertionSequence >
      void operator() ( const SpeciesIterator & sbegin,
                        const SpeciesIterator & send,
                        BackInsertionSequence & result_list,
                        InteractionInfo & info,
                        const RuntimeDB<options> & db,
                        const SpeciesMap & map = SpeciesMap() ) {
        using chimp::interaction::velocity; // generic accessor for velocity

        typedef std::iterator_traits<SpeciesIterator>::value_type Species;
        typedef typename Species::iterator PIter; /* ParticleIterator */

        typedef RuntimeDB<options>::InteractionTable::const_iterator IIter;
        for ( IIter i  = db.getInteractions().begin();
                    i != db.getInteractions().end(); ++i ) {
          const int & A = i->lhs.A;
          const int & B = i->lhs.B;

          if (i->rhs.size() == 0)
            /* no interactions for these inputs. */
            continue;

          /* now let's try and pick a mock collision pair for this
           * interaction. */
          if ((A == B && sbegin[A].size() < 2) ||
              (sbegin[A].size() == 0 || sbegin[B].size() == 0))
              continue;

          i->print(std::cout << "Performing test interactions for:", db) << '\n';

          double m_s_v = info.maxSigmaVelProduct(A,B, db);


          /* Start by determining the number of collisions to use. */
          int number_of_collisions_to_test = XXX;

          while ( number_of_collisions_to_test ) {

            /* Pick the particles that need to interact. */
            Particle::list::iterator pi
              = sbegin[A].begin() + int(sbegin[A].size()*MTRNGrand()*0.99999999);
            Particle::list::iterator pj = pi;
            while ( pi == pj )
              pj = sbegin[B].begin()
                 + int(sbegin[B].size()*MTRNGrand()*0.99999999);

            /* Relative velocity of the the two particles. */
            double v_rel = (velocity(pi) - velocity(pj)).abs();

            /* This calculates the proper output path, observing both absolute
             * probability that any interaction occurs as well as relative probabilities
             * for when an interaction does occur.  
             *
             * The .first<int> component is the interaction index, such as
             * i->rhs[path.first] to obtain the Equation instance for the interaction
             * that should occur.  
             * The .second<double> component is the cross section value of this
             * particular interaction for the given relative velocity.   */
            std::pair<int,double> path = i->calculateOutPath(m_s_v, v_rel);

            if ( path.first < 0 )
              // FIXME:  How should we allow statistics to be gathered for this?
            else {
              // FIXME:  How should we allow statistics to be gathered for this?

              /* and now do the interaction for the two different particles.  */
              // FIXME:  The actual thing to do here really might depend on the
              // algorithm begin implemented (DSMC, collisions with background gas,
              // rates calculations, etc.)
            }

            /* one down, ... more to go. */
            --number_of_collisions_to_test;
          }/* while doing colllision tests */
        }
      }/* operator() */

    };

  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimplib_examples_simtest_opDSMC_h
