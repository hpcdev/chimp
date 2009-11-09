/** \file
 * Implementation of cross_section::DATA xml load function.
 * */

#include <chimp/interaction/cross_section/DATA.h>

#include <olson-tools/xml/physical_parse.h>

namespace chimp {
  namespace interaction {
    namespace cross_section {

      using runtime::physical::Quantity;
      using runtime::physical::constant::si::eV;
      using runtime::physical::unit::m;
      using runtime::physical::unit::s;
      using runtime::physical::unit::kg;
      using namespace runtime::physical::math;

      typedef olson_tools::data_set<Quantity,Quantity> pqdata_set;

      DoubleDataSet loadCrossSectionData( const xml::Context & x,
                                          const ReducedMass & mu ) {
        const Quantity m_s = m/s;
        const Quantity m2  = m*m;
        DoubleDataSet table;
        pqdata_set pqd = x.parse<pqdata_set>();
        /* convert x-val to velocity units. */
        for (pqdata_set::iterator i = pqd.begin(); i != pqd.end(); i++) {
          Quantity v = i->first;
          if (v.units == eV.units)
            v = sqrt(v / (mu.value * kg) );

          register double v_coeff, m2_coeff;
          v.assertMatch(m_s).getCoeff(v_coeff);
          i->second.assertMatch(m2).getCoeff(m2_coeff);

          table.insert( std::make_pair( v_coeff, m2_coeff ) );
        }

        return table;
      }

    } /* namespace chimp::interaction::cross_section */
  } /* namespace chimp::interaction */
} /* namespace chimp */