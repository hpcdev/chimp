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
 * Declaration of interaction::model::InElastic class.
 */

#ifndef chimp_interaction_model_InElastic_h
#define chimp_interaction_model_InElastic_h

#include <chimp/interaction/Equation.h>
#include <chimp/interaction/model/Base.h>

#include <xylose/power.h>
#include <xylose/Vector.h>
#include <xylose/xml/Doc.h>
#include <xylose/compat/math.hpp>

#include <string>
#include <vector>

namespace chimp {
  namespace interaction {
    namespace model {

      /** Implementation of an <b>in</b>elastic interaction model. */
      template < typename options >
      struct InElastic : Base<options> {
        /* TYPEDEFS */
        typedef typename options::Particle Particle;


        /* STATIC STORAGE */
        static const std::string label;



        /* MEMBER FUNCTIONS */
        /** Virtual NO-OP destructor. */
        virtual ~InElastic() { }

        /** Obtain the label of the model. */
        virtual std::string getLabel() const {
          return label;
        }

        /** load a new instance of the Interaction. */
        virtual InElastic * new_load( const xml::Context & x,
                                      const interaction::Equation<options> & eq,
                                      const RuntimeDB<options> & db ) const {
          return new InElastic/*( eq.reducedMass )*/;
        }

      };

      template < typename options >
      const std::string InElastic<options>::label = "inelastic";

    } /* namespace chimp::interaction::model */
  } /* namespace chimp::interaction */
} /* namespace chimp */

#endif // chimp_interaction_model_InElastic_h
