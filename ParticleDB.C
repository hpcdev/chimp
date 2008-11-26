// -*- c++ -*-
// $Id$
/*
 * Copyright 2000-2008 Spencer Olson
 *
 */

#include "ParticleDB.h"
#include <string.h>


namespace particledb {

/* TODO:
    Create a consistent and simple interface to different types of
    interactions which each have certain cross sections.  These interactions
    can include multiple types of state-changing (inelastic) collisions,
    ionizing collisions, and so forth.  These interactions would include: 
        1.  cross sections, probabilities, etc.
        2.  Path information with regards to inelastic collisions.  The path
        could either be internal quantum state changes, association, three-body
        recombination (2 particles combining with a background electron gas),
        dissociation, and so on.  The path implementation might result in
        particles changing type as they collide--with a separate type given
        for each particle with a different charge, isotope, internal quantum
        number, and so on.  It would be best for the database to organize
        these related types such that the paths are very easy to traverse.
        Easy to traverse would be something like a pointer arithmatic, or
        perhaps traversal of a dual-linked list of items.  
        3.  Pre-requisites for the interaction to be allowed:
            a.  Threshold energy
            b.  State dependency
            c.  ???
    Essentially, these different interactions would represent different
    collision models.  

    TODO:
    Create a consistent and simple interface to state-type information to
    particles.  State type of information would be something like:
        1.  Internal quantum state
        2.  charge
*/

} /* namespace particledb */

