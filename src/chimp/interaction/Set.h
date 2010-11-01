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


/** \file
 * Definition of the equation set where each Equation has the same Input.
 * */

#ifndef chimp_interaction_Set_h
#define chimp_interaction_Set_h

#include <chimp/interaction/Equation.h>

#include <xylose/logger.h>
#include <xylose/compat/math.hpp>

#include <vector>
#include <ostream>

namespace chimp {
  namespace interaction {

    /** A set of detailed balanced interaction equations that share a common
     * left-hand-side portion. */
    template < typename options >
    struct Set {
      /* TYPEDEFS */
      typedef interaction::Equation<options> Equation;

      typedef typename Equation::list eq_list;

      /** Return type of calcualteOutPath.
       * OutPath.first:  the index of the output interaction.<br>
       * OutPath.second:  the cross-section value for this output path.
       * */
      typedef std::pair<int, double> OutPath;



      /* MEMBER STORAGE */
      /** The left hand side of a set of several equations. */
      Input lhs;

      /** A set of right hand sides of the several equations. */
      eq_list rhs;



      /* MEMBER FUNCTIONS */
      /** Constructor.  A blank set of equations are created if constructor
       * arguments are omitted. */
      Set(const Input & lhs = Input(), const eq_list & rhs = eq_list())
        : lhs(lhs), rhs(rhs) {}

      /** Print the common left hand side followed by each of the equations. */
      template <class RnDB>
      std::ostream & print(std::ostream & out, const RnDB & db) const {
        out << "{\n"
               "\tInput : "; lhs.print(out,db) << "\n";

        for ( unsigned int i = 0; i < rhs.size(); ++i ) {
          out << "\tOutput #" << i << ":  ";
          rhs[i].print(out,db) << '\n';
        }

        return out << '}';
      }

      /** Find the local maximum of cross-section*velocity (within a given
       * range of velocity space).  This version of the Set class returns the
       * sum of the maxima of each cross section contained in the set.  For null
       * collision methods, this will resort to more collisions than necessary.
       * For a more performance sensitive implementation, use the precomputed
       * Set implementation.
       * */
      inline double findMaxSigmaVProduct(const double & v_rel_max) const {
        double sum = 0.0;
        for ( typename eq_list::const_iterator i = rhs.begin(),
                                             end = rhs.end();
                                              i != end; ++i ) {
          sum += i->cs->findMaxSigmaV(v_rel_max).first;
        }
        return sum;
      }

      /** Chooses an interaction path to traverse dependent on the incident
       * relative speed and the current value of (sigma*relspeed)_max. 
       *
       * The implementation here actually calculates the probabilities
       * fully during runtime. 
       *
       * @return The index of the right-hand-side of the interaction
       * equation is returned, unless no interaction can be performed.  In
       * this latter case, a value of -1 will be returned.
       * */
      template < typename RNG >
      std::pair<int,double>
      calculateOutPath( double & max_sigma_relspeed,
                        const double & v_relative,
                        RNG & rng ) const {
        /* first find the normalization factor for the sum of
         * cross-section values at this velocity. */
        double cs_tot = 0;
        double cs_max = 0;
        std::vector<double> cs;
        cs.reserve(rhs.size());
        for ( typename eq_list::const_iterator i = rhs.begin(),
                                             end = rhs.end();
              i != end; ++i ) {
          double csi = i->cs->operator()(v_relative);
          cs_tot += csi;
          cs.push_back(csi);

          if (csi > cs_max)
            cs_max = csi;
        }

        /* now evaluate whether any of these interactions should even
         * happen. */
        if ( (rng.rand() * max_sigma_relspeed) > (cs_tot*v_relative) )
          return std::make_pair(-1,0.0); /* no interaction!!! */

        /* upgrade max_sigma_relspeed? */
        {
          register double cs_max_relspeed = cs_max * v_relative;
          if (cs_max_relspeed > max_sigma_relspeed)
            max_sigma_relspeed = cs_max_relspeed;
        }

        /* now, we finally pick our output state.  */
        double r = rng.randExc() * cs_tot;
        cs_tot = 0;
        int j = 0;
        for ( std::vector<double>::iterator i = cs.begin(),
                                          end = cs.end();
              i < end; ++i, ++j) {
          cs_tot += (*i);
          if (cs_tot > r)
            return std::make_pair(j,(*i));
        }

        /* we actually better never get here. */
        xylose::logger::log_severe("interaction::Set::calculateOutPath reached invalid return");
        return std::make_pair(-1,0.0);
      }

      template < typename PIter,
                 typename BackInsertionSequence,
                 typename RNG >
      std::pair<int,double>
      interact( double & max_sigma_relspeed,
                const std::pair<PIter, PIter> & pair,
                BackInsertionSequence & result_list,
                RNG & rng ) const {
        typename options::Particle & pA = *pair.first;
        typename options::Particle & pB = *pair.second;

        using chimp::accessors::particle::velocity;
        using chimp::accessors::particle::species;

        /* Relative velocity of the the two particles. */
        double v_rel = ( velocity(pA) - velocity(pB) ).abs();

        std::pair<int,double> path =
          calculateOutPath( max_sigma_relspeed, v_rel, rng );

        if ( path.first >= 0 ) {
          /* help make sure that the order of the particles is correct--sorted
           * by increasing mass. */
          if ( species(pA) > species(pB) )
            rhs[path.first].interaction->interact( pB, pA, result_list, rng );
          else
            rhs[path.first].interaction->interact( pA, pB, result_list, rng );
        }

        return path;
      }
    };

#if 0
    /** This set implementation pre-calculates the cross-section and
     * probabilities at various pre-determined energies and stores the
     * associated lookup-table for later use. */
    struct PreComputedSet : Set {

      struct elt {
        double cross_section_total;
        std::vector<double> cross_sections;
        xylose::Distribution distrib;
      };


      std::pair<int,double>
      calculateOutPath( double & max_sigma_relspeed,
                        const double & v_relative ) const {
        if (rhs.empty())
          return std::make_pair(-1,0.0);
        return 1;
      }
    };
#endif

    template < typename options >
    inline bool hasElastic( const Set<options> & set ) {
      return hasElastic(set.rhs);
    }

    template < typename options >
    inline typename Set<options>::Equation::list::const_iterator
    getElastic( const Set<options> & set ) {
      return getElastic(set.rhs);
    }


  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_Set_h
