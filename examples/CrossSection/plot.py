#!/usr/bin/env python

# This will help plot a whole bunch of cross sections, assuming that each is 100
# lines long.  Each cross section curve will be plotted with a different color.

import sys
import gobject
import matplotlib
matplotlib.use('GTKAgg')

from pylab import *

class UpdateData:
    def __init__(self, filename='data.dat', N_points=100, N_lines=10000):
        self.N_points = N_points
        self.N_lines = N_lines
        self.d = loadtxt(filename)
        self.current_plot = 0
        self.fig = figure()
        self.fig.hold(False)

    def update(self, *args):
        retval = True

        self.fig.clf()
        ax = self.fig.add_subplot(111)

        for i in xrange(self.N_lines):
            start = self.current_plot * self.N_points
            end   = start + self.N_points

            if start >= len(self.d):
                retval = False
                #self.current_plot = 0
                break

            ax.semilogy(self.d[start:end,0], self.d[start:end,1])
            self.current_plot += 1

        ax.set_ybound(1e-6,.5)
        self.fig.canvas.draw()


        return retval

def usage(arg0):
    print \
    """
    usage:  {arg0} [points=<number-of-points>] [plots=<number-of-plots>]

        Where:
            <number-of-points> :  Number of data points saved by getCrossSection
                                  [Default:  100]
            <number-of-plots>  :  Number of cross sections to put on a single
                                  plot.  The figure will cycle through each of
                                  the sub-sets of data.  
                                  [Default:  all]
    """.format(arg0=arg0)
    

def main():
    args = {}
    try:
        for arg in sys.argv[1:]:
            s = arg.split('=')
            if   s[0] == 'points':
                args['N_points'] = int(s[1])
            elif s[0] == 'plots':
                args['N_lines'] = int(s[1])
            elif arg == '--help':
                usage(sys.argv[0])
                return
            else:
                raise ValueError('could not parse option:  ' + arg)
    except Exception, e:
        print 'error: ', e
        usage(sys.argv[0])
        return

    ud = UpdateData(**args)
    gobject.timeout_add(1550, ud.update)
    show()


if __name__ == '__main__':
    main()

