
#ifndef chimp_interaction_Set_h
#define chimp_interaction_Set_h

#include <chimp/interaction/Equation.h>

#include <olson-tools/random/random.h>
#include <olson-tools/logger.h>

#include <cmath>
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
       * range of velocity space.
       * */
      inline double find_max_sigma_v_rel(const double & v_rel_max) const {
        double mx = 0.0;
        for ( typename eq_list::const_iterator i = rhs.begin();
              i != rhs.end(); ++i ) {
          mx = std::max(mx, i->cs->find_max_sigma_v_rel(v_rel_max));
        }
        return mx;
      }

      /** Find the local maximum of cross-section*velocity assuming an
       * ensemble of particles with a given temperature.
       * */
      inline double find_max_sigma_v_rel_from_stddev_v(const double & stddev_v) const {
        return find_max_sigma_v_rel(CrossSection::MAX_SPEED_FACTOR * stddev_v);
      }

      /** Find the local maximum of cross-section*velocity assuming an
       * ensemble of particles with a given temperature.
       * */
      inline double find_max_sigma_v_rel_from_T(const double & T) const {
        return find_max_sigma_v_rel_from_stddev_v(stddev_v_rel(T, lhs.mu_AB));
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
      std::pair<int,double>
      calculateOutPath( double & max_sigma_relspeed,
                        const double & v_relative ) const {
        /* first find the normalization factor for the sum of
         * cross-section values at this velocity. */
        double cs_tot = 0;
        double cs_max = 0;
        std::vector<double> cs;
        cs.reserve(rhs.size());
        for ( typename eq_list::const_iterator i = rhs.begin();
              i != rhs.end(); ++i ) {
          double csi = i->cross_section(v_relative);
          cs_tot += csi;
          cs.push_back(csi);

          if (csi > cs_max)
            cs_max = csi;
        }

        /* now evaluate whether any of these interactions should even
         * happen. */
        if ( (MTRNGrand() * max_sigma_relspeed) > (cs_tot*v_relative) )
          return std::make_pair(-1,0); /* no interaction!!! */

        /* upgrade max_sigma_relspeed? */
        {
          double cs_max_relspeed = cs_max * v_relative;
          if (cs_max_relspeed > max_sigma_relspeed)
            max_sigma_relspeed = cs_max_relspeed;
        }

        /* now, we finally pick our output state.  The 0.999999999 factor
         * is to ensure that r < cs_tot. */
        double r = MTRNGrand() * cs_tot * 0.999999999;
        cs_tot = 0;
        int j = 0;
        for (std::vector<double>::iterator i = cs.begin(); i < cs.end(); ++i, ++j) {
          cs_tot += (*i);
          if (cs_tot > r)
            return std::make_pair(j,(*i));
        }

        /* we actually better never get here. */
        olson_tools::logger::log_severe("interaction::Set::calculateOutPath reached invalid return");
        return std::make_pair(-1,0);
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
        olson_tools::Distribution distrib;
      };


      std::pair<int,double>
      calculateOutPath( double & max_sigma_relspeed,
                        const double & v_relative ) const {
        if (rhs.empty())
          return std::make_pair(-1,0);
        return 1;
      }
    };
#endif

  }/* namespace particldb::interaction */
}/* namespace particldb */

#endif // chimp_interaction_Set_h
