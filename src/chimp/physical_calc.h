/** \file
 * Prototype for physical::calculator preparation function.
 */
#ifndef chimp_physical_calc_h
#define chimp_physical_calc_h

#include <olson-tools/xml/Doc.h>

namespace chimp {
  namespace xml = olson_tools::xml;

  /** Prepare the units calculator by executing commands stored in the
   * calc-command subsections of the xml file.
   * This function is used inside the RuntimeDB class after the xml data set is
   * opened.  Generally, an end user does not need to use this function. 
   */
  void prepareCalculator(const xml::Doc & doc);
} /* namespace chimp */

#endif // chimp_physical_calc_h
