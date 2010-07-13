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


#ifndef chimp_interaction_detail_DriverRetval_h
#define chimp_interaction_detail_DriverRetval_h

namespace chimp {
  namespace interaction {
    namespace detail {

      /** Implementation of OUT-OF-PLACE interactions!! */
      template < bool InplaceInteractions >
      struct DriverRetval;


      /** Implementation of OUT-OF-PLACE interactions!! */
      template <>
      struct DriverRetval<false> {
        template < typename Path, typename PIter, typename Iq, typename Eq,
                   typename Range >
        void operator() ( const Path & path,
                          const std::pair<PIter, PIter> & pair,
                          const Iq & iq,
                          const size_t & iq_sz_i,
                          Eq & eq,
                          const unsigned int & A,
                          const unsigned int & B,
                          Range & aRange,
                          Range & bRange
                        ) { }
      };




      /** Implementation of IN-PLACE interactions!! */
      template <>
      struct DriverRetval<true> {
        template < typename Path, typename PIter, typename Iq, typename Eq,
                   typename Range >
        void operator() ( const Path & path,
                          const std::pair<PIter, PIter> & pair,
                          const Iq & iq,
                          const size_t & iq_sz_i,
                          Eq & eq,
                          const unsigned int & A,
                          const unsigned int & B,
                          Range & aRange,
                          Range & bRange
                        ) {

          if ( path.first < 0 || iq.size() <= iq_sz_i )
            return;

          /* the path IS valid AND products were created:
           *    remove the initial pair--if no products were created, we assume
           *    the interaction operated on the particles in place.
           */
          PIter last = aRange.end() -1;
          eq.insert( last ); // queue the deletion

          if ( pair.second == last ) {
            /* we have to take care of this one special case */
            assert( A == B );
            --last;
            *pair.first = *last;
          } else {
            *pair.first = *last; // move the last up
            /* set the new range for A species. */
            aRange = SpeciesRange( aRange.begin(), last );

            last = bRange.end() -1;
            *pair.second = *last; // move the last up
          }

          eq.insert( last ); // queue the deletion
          /* set the new range for B species. */
          bRange = SpeciesRange( bRange.begin(), last );

        }
      };

    }/* namespace chimp::interaction::detail */
  }/* namespace chimp::interaction */
}/* namespace chimp */

#endif // chimp_interaction_detail_DriverRetval_h
