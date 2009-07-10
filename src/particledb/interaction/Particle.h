// -*- c++ -*-
// $Id: Particle.h,v 1.3 2005/06/07 10:00:14 olsonse Exp $
/*
 * Copyright 2000-2004 Spencer Olson
 *
 * $Log: Particle.h,v $
 * Revision 1.3  2005/06/07 10:00:14  olsonse
 * Many changes over the course of time
 *
 * Revision 1.2  2005/01/19 11:44:36  olsonse
 * Added Argon data.
 * Added lines to compile on SunOS.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

#ifndef particledb_interaction_Particle_h
#define particledb_interaction_Particle_h

#  include <olson-tools/random/random.h>
#  include <olson-tools/Vector.h>

#  include <ostream>


namespace particledb {
  namespace interaction {
    using olson_tools::Vector;

    /** Just some class to represent particles to sort. */
    struct Particle {
      /* MEMBER STORAGE */
      /** Position array.  */
      Vector<double,3> x;

      /** Velocity array.  */
      Vector<double,3> v;


      /* MEMBER FUNCTIONS */
      Particle( const Vector<double,3> & x = 0.0,
                const Vector<double,3> & v = 0.0 ):
        x(x), v(v) { }

    };// Particle



    /** Assign random values to the particle's x and v members. */
    inline void randomize( Particle & p,
                           const Vector<double,3> & dx = 100,
                           const Vector<double,3> & dv = 100 ) {
      /* random values */
      p.x[0] = dx[0] * ( MTRNGrand() - .5 );
      p.x[1] = dx[1] * ( MTRNGrand() - .5 );
      p.x[2] = dx[2] * ( MTRNGrand() - .5 );
    
      p.v[0] = dv[0] * ( MTRNGrand() - .5 );
      p.v[1] = dv[1] * ( MTRNGrand() - .5 );
      p.v[2] = dv[2] * ( MTRNGrand() - .5 );
    }
    
    /** Compute the distance between a Particle and a point. */
    inline double distance( Particle & p, const Vector<double,3> & r ) {
      return (p.x - r).abs();
    }

    /** Compute the distance between two Particle instances. */
    inline double distance( const Particle & a, const Particle & b ) {
      return (a.x - b.x).abs();
    }

    /** Print the six position and velocity values out seperated by spaces. */
    inline std::ostream & operator<< ( std::ostream & output,
                                       const Particle & q ) {
      return output << q.x << q.v;
    }
    
    /** Read in the six position and velocity values into Particle class. */
    inline std::istream & operator>> ( std::istream & input, Particle & p ) {
      return input >> p.x >> p.v;
    }

  }/* namespace particledb::interaction */
}/* namespace particledb */

#endif // particledb_interaction_Particle_h
