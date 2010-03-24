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


#ifndef chimp_interaction_Driver_h
#define chimp_interaction_Driver_h

#include <chimp/interaction/selectRandomPair.h>
#include <chimp/accessors.h>

#include <olson-tools/random/random.h>

#include <iterator>

namespace chimp {
  namespace interaction {

    /** Default generator and updater of
     * \f$ \left( \sigma_{\rm T} v_{\rm rel} \right)_{\rm max} \f$.
     * This default implementation requires the CrossSpeciesInfo class to have a
     * maxRelativeVelocity function that accepts species A and B (as integer
     * parameters) and returns the maximum relative velocity of species A and B.
     * This value as provided by the CrossSpeciesInfo class can be from a
     * tracked quantity, an estimated quantity, or whatever.
     */
    template < typename ChimpDB,
               typename CrossSpeciesInfo >
    struct DefaultMaxSigmaVProduct {

      const ChimpDB & db;
      const CrossSpeciesInfo & info;

      DefaultMaxSigmaVProduct( const ChimpDB & db,
                               const CrossSpeciesInfo & info )
        : db(db), info(info) { }

      /** Calculate the maximum value of 
       * \f$ \left( \sigma_{\rm T} v_{\rm rel} \right)_{\rm max} \f$.
       */
      double get( const int & A,
                  const int & B ) const {
        return
        db(A,B).findMaxSigmaVProduct(
          info.maxRelativeVelocity(A,B)
        );
      }

      /** Updates the maximum value of 
       * \f$ \left( \sigma_{\rm T} v_{\rm rel} \right)_{\rm max} \f$.
       * This implemenation does nothing.
       */
      void update( const int & A,
                   const int & B,
                   const double & m_s_v ) const { }
    };

    /** The default collision monitor does nothing. */
    struct NullMonitor {
      template < typename ChimpDB,
                 typename PIter >
      void interactions( const ChimpDB & db,
                         const std::pair<PIter, PIter> & pair,
                         const std::pair<int,double> & path ) const { }

      void pairtests( const double & number_of_pairtests ) const { }
    };


    /** Driver class for performing all interactions necessary for all the types
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
    template < typename CrossSpeciesInfo,
               typename ChimpDB,
               typename Monitor = NullMonitor,
               typename MaxSigmaVProduct
                 = DefaultMaxSigmaVProduct< ChimpDB, CrossSpeciesInfo > >
    struct Driver {
      /* MEMBER STORAGE */
      const CrossSpeciesInfo & info;
      const ChimpDB & db;
      Monitor monitor;


      /* MEMBER FUNCTIONS */
      Driver( const CrossSpeciesInfo & info,
              const ChimpDB & db,
              const Monitor & monitor = Monitor() )
        : info(info), db(db), monitor( monitor ) { }

      template < typename SpeciesIterator,
                 typename BackInsertionSequence >
      void operator() ( const double & dt,
                        const SpeciesIterator & sc,
                        BackInsertionSequence & result_list ) {
        using chimp::accessors::particle::velocity; // generic accessor for velocity

        typedef typename std::iterator_traits<SpeciesIterator>::value_type Species;
        typedef typename Species::iterator PIter; /* ParticleIterator */

        MaxSigmaVProduct maxSigmaVProduct(db, info);

        typedef typename ChimpDB::InteractionTable::const_iterator IIter;
        for ( IIter i  = db.getInteractions().begin(),
                   end = db.getInteractions().end();
                    i != end; ++i ) {
          const int & A = i->lhs.A.species;
          const int & B = i->lhs.B.species;

          if (i->rhs.size() == 0)
            /* no interactions for these inputs. */
            continue;

          /* now let's try and pick a mock collision pair for this
           * interaction. */
          if ((A == B && sc[A].size() < 2) ||
              (sc[A].size() == 0 || sc[B].size() == 0))
              continue;

          double m_s_v = maxSigmaVProduct.get(A,B);

          /* Start by determining the number of collisions to use.
           * N_test = Fa Fb Na Nb dt MAX(s v) / ( 2 V min(Fa,Fb) )
           */
          double number_of_collisions_to_test;
          {/* FIXME:  support variable weights per particles... */
            using chimp::accessors::particle::weight;
            register double wA = weight(*sc[A].begin()),
                            wB = weight(*sc[B].begin());

            number_of_collisions_to_test =
               wA * wB * sc[A].size() * sc[B].size() * dt * m_s_v
              / ( 2 * info.volume() * std::min( wA, wB ) )
            ;
          }

          {/* Promote the remaining selection probablity to either 0 or 1 */
            register double number_of__fraction =
              number_of_collisions_to_test -
              static_cast<int>(number_of_collisions_to_test);

            using olson_tools::random::MTRNGrand;
            if ( MTRNGrand() < number_of__fraction )
              number_of_collisions_to_test += 1.0;
          }

          monitor.pairtests( number_of_collisions_to_test );

          while ( number_of_collisions_to_test > 0 ) {
            typedef std::pair<PIter, PIter> CollisionPair;

            CollisionPair pair = selectRandomPair( sc[A], sc[B] );

            // Picks the correct output equation and uses it...
            monitor.interactions(
              db, pair,
              i->interact( m_s_v, pair, result_list )
            );

            /* one down, ... more to go. */
            number_of_collisions_to_test -= 1.0;
          }/* while doing colllision tests */

          /* allow for the calling code to tack m_s_v and update it. */
          maxSigmaVProduct.update(A, B, m_s_v);

        }/* for all interactions */
      }/* operator() */

    };

  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_Driver_h
