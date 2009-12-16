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
 * The main documentation page for doxygen. 
 */

/** \mainpage CHIMP API Documentation

<h2>Chemical Interactions and Materials Properties Library</h2>


<h3>Introduction</h3>
Currently, it is very common for different simulations to use slightly, if not
drastically, different physical data for the same materials.  This creates both
confusion as to how to compare the results of different simulations, but also
questions the validity of any one simulation.  There is a current need in the
computational physics community for a common repository of physical data and a
means to deliver that data to simulation tools.  This work represents a
collaborative effort to develop a database and library to provide physical data
and associated model calculations in a consistent, simple yet flexible manner.

The scope of the data to be housed within the database is intended to be
necessarily wide, so as to create some uniformity of simulation results based on
the same data.  All data to be added to any released version of the library will
be required to be both publicly accessible and well documented as to its origin.
The success of this work will rely on heavily on all collaborators contributing
data in the correct format and helping thoroughly documenting this data. 

This manual constitutes the technical reference or application programming
interface (API) documentation for CHIMP.  This manual is automatically generated
using the doxygen code documentation tool.  Currently, we support HTML and
\f$\textnormal{\LaTeX}\f$ output generation.  Please follow the appropriate links to
documentation for each function, class, and namespace of the API.


<h3>Additional Material</h3>
  Please refer to the following material for current status, future direction,
  and otherwise typical package information:
  - \subpage chimp_discussions
  - \subpage chimp_platforms
  - \subpage chimp_license
  - \subpage chimp_readme
  - \subpage chimp_install
  - \subpage chimp_changelog
  - \subpage chimp_authors
  .
*/



//-----------------------------------------------------------
/** \page chimp_license Licence
    <h2><a name="lgpl">LGPL v3.0</a>
      (See the <a href="#gpl">GPL License</a> as LGPL is a set of
      exceptions/extensions to it)
    </h2>
    \verbinclude COPYING.LESSER

    <h2><a name="gpl">GPL v3.0</a>
      (See the <a href="#lgpl">LGPL License</a> under which this package is
      released)
    </h2>
    
    \verbinclude COPYING
*/



//-----------------------------------------------------------
/** \page chimp_readme README
    \verbinclude README
*/



//-----------------------------------------------------------
/** \page chimp_install INSTALL
    \verbinclude INSTALL
*/



//-----------------------------------------------------------
/** \page chimp_changelog ChangeLog
    \verbinclude ChangeLog
*/



//-----------------------------------------------------------
/** \page chimp_authors AUTHORS
    \verbinclude AUTHORS
*/


//-----------------------------------------------------------
/** \page chimp_discussions Direction and Discussions

If appropriate public information exists (e.g. in journals or databases such as
from NIST), the level of fidelity will also be documented.  
A recent discussion among AFRL/DOE collaborators brought out the following
idea for documenting fidelity of the data that we will be distributing.
  - Baked from thin air
  - Published
  - Published works cited
  - Experimental data claimed to have compared favorably
  - etc.
  .
*/


//-----------------------------------------------------------
/** \page chimp_platforms Supported Platforms
  <table border=1>
    <tr><th>Platform</th><th>Compiler</th><th>Version</th><th>Status</th></tr>
    <tr><td rowspan="2">Linux</td><td rowspan="2">GCC</td><td>4.1.2 (RedHat/Centos 5.3)</td><td>Supported</td></tr>
    <tr>                                                  <td>4.4.1 (Ubuntu 9.10)</td><td>Supported</td></tr>
    <tr><td rowspan="2">Cygwin</td><td rowspan="2">GCC</td><td>3.5</td><td>Supported</td></tr>
    <tr>                                                  <td>4.x</td><td>Not Supported</td></tr>
  </table>
*/
