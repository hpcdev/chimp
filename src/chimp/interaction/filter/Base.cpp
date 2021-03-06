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

#include <chimp/interaction/filter/Or.h>
#include <chimp/interaction/filter/And.h>
#include <chimp/interaction/filter/Not.h>
#include <chimp/interaction/filter/Null.h>
#include <chimp/interaction/filter/EqIO.h>
#include <chimp/interaction/filter/Label.h>
#include <chimp/interaction/filter/Section.h>
#include <chimp/interaction/filter/Elastic.h>
#include <chimp/interaction/filter/Base.h>

#include <xylose/xml/Doc.h>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <map>
#include <string>

namespace chimp {
  namespace interaction {
    namespace filter {

      namespace loader {
        std::map< std::string, shared_ptr<filter::loader::Base> > list;

        int init() {
          namespace F = chimp::interaction::filter;
          namespace L = chimp::interaction::filter::loader;
          using boost::make_shared;
          loader::list[F::Or::label()]      = make_shared<L::Or>();
          loader::list[F::And::label()]     = make_shared<L::And>();
          loader::list[F::Not::label()]     = make_shared<L::Not>();
          loader::list[F::Null::label()]    = make_shared<L::Null>();
          loader::list[F::EqIO::label()]    = make_shared<L::EqIO>();
          loader::list[F::Label::label()]   = make_shared<L::Label>();
          loader::list[F::Section::label()] = make_shared<L::Section>();
          loader::list[F::Elastic::label()] = make_shared<L::Elastic>();

          return loader::list.size();
        }

        static const int _init = loader::init();
        int dummy_function_to_shut_up_compiler() {
          return _init;
        }
      }/* namespace chimp::interaction::filter::loader */


      using boost::shared_ptr;
      namespace xml = xylose::xml;

      /** Need to parse the filter set from xml. */
      void parse_item( shared_ptr<filter::Base> & out,
                       const xml::Context & x ) {
        xml::Context::list x_list = x.eval("child::*");// get all children

        if ( x_list.size() == 0u ) {
          out = shared_ptr<filter::Base>( new Null );
        } else if ( x_list.size() > 1u )
          throw xml::error(
            "<equation-filter> should only have one direct child node"
          );
        else {
          xml::Context & xchild = x_list.front();
          out = filter::loader::lookup( xchild.name() )->load(xchild);
        }
      }

    }/* namespace chimp::interaction::filter */
  }/* namespace chimp::interaction */
}/* namespace chimp */

