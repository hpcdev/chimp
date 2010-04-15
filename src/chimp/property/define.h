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
 * Implementation of a generic property class.
 */

#ifndef chimp_property_define_h
#define chimp_property_define_h

#include <chimp/property/detail/check.h>

#include <xylose/xml/Doc.h>

#include <physical/runtime.h>

#include <string>

namespace chimp {
  namespace property {
    namespace dim = runtime::physical::dimension;

    #define CHIMP_DEFINE_PARTICLE_PROPERTY( \
      Property, T, dimension, xpath, default_value ) \
        CHIMP_DEFINE_PARTICLE_PROPERTY_IMPL( \
          Property, T, dimension, xpath, default_value, false )

    #define CHIMP_DEFINE_REQUIRED_PARTICLE_PROPERTY( \
      Property, T, dimension, xpath) \
        CHIMP_DEFINE_PARTICLE_PROPERTY_IMPL( \
          Property, T, dimension, xpath, T(), true )

    #define CHIMP_DEFINE_PARTICLE_PROPERTY_IMPL( \
      Property, T, dimension, xpath, default_value, required ) \
    struct Property { \
      /* TYPEDEFS */ \
      typedef T value_type; \
 \
      /* MEMBER STORAGE */ \
      T value; \
 \
      /* MEMBER FUNCTIONS */ \
      /** Constructor which sets the value to the given parameter. */ \
      Property(const T & val = default_value) : value(val) {} \
 \
      /** Stream printer. */ \
      std::ostream & print(std::ostream & out, const std::string & sep) const { \
        return out << xpath << ": " << value << sep; \
      } \
 \
      /** Load function (loads from xml context. */ \
      static Property load(const xylose::xml::Context & x) { \
        namespace cpd = chimp::property::detail; \
        if (required) \
          return x.query< cpd::check<T,dimension> >(xpath).value; \
        else \
          return x.query< cpd::check<T,dimension> > \
            ( xpath, default_value ).value; \
      } \
    } /* require the ';' by calling code */

  }/* namespace chimp::property */
}/*namespace chimp */

#endif // chimp_property_define_h
