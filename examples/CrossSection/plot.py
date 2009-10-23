#!/usr/bin/env python

# This will help plot a whole bunch of cross sections, assuming that each is 100
# lines long.  Each cross section curve will be plotted with a different color.

import gobject
import matplotlib
matplotlib.use('GTKAgg')

from pylab import *

class UpdateData:
    def __init__(self, filename='data.dat', N_points=100):
        self.N_points = N_points
        self.d = loadtxt(filename)
        self.current_plot = 0
        self.fig = figure()
        self.fig.hold(False)

    def update(self, *args):
        retval = True

        self.fig.clf()
        ax = self.fig.add_subplot(111)

        for i in xrange(10000):
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

ud = UpdateData()

gobject.timeout_add(1550, ud.update)

show()

