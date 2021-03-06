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
 * Dummy file for documenting some of the general namespaces. 
 */

#ifndef chimp___namespace___h
#define chimp___namespace___h

/** Namespace for the Chimp library. */
namespace chimp {

  /** Classes, types, and functions for Interactions. */
  namespace interaction {

    /** Cross section data models. */
    namespace cross_section { }

    /** Filters for selecting appropriate interactions. */
    namespace filter { }

    /** Default Functional models for interactions supported by Chimp. */
    namespace model { }
  }

  /** Properties of materials in various states of matter (gas, solid, ...). */
  namespace property { }
}

#endif // chimp___namespace___h
