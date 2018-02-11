#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import threading
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/../../computer/plume_api/python')

from plume_api import *

class PlumeEmitterLed(object):
    """
    This class generate a clean animation for the led of the emitter.
    """
    def __init__(self, plumeEmitter):
        self.plumeEmitter = plumeEmitter
    
        self.toAdd = 2
        self.BC = 0

        self.plumeEmitter.startLed()
        self.setState(0)

        self.needToExit = False
        self.thread = threading.Thread(None, self.run, "Led")
        self.thread.daemon = True
        self.thread.start()

    def setState(self, state):
        """
        state 0 : Red
        state 1 : Blue
        """
        octant = np.zeros([2, 2, 2], dtype=('u1,u1,u1'))

        for x in range(2):
          for y in range(2):
            for z in range(2):
              octant[x, y, z][0] = 255 if state == 0 else 0 # Red
              octant[x, y, z][1] = 0
              octant[x, y, z][2] = 255 if state == 1 else 0 # Blue

        self.plumeEmitter.setLed(octant)

    def run(self):
        while not self.needToExit:
            self.BC += self.toAdd

            if self.BC > 201:
                self.BC = 200
                self.toAdd = -2
            elif self.BC < 0:
                self.BC = 1
                self.toAdd = 2

            self.plumeEmitter.setLedBrightless(self.BC)
            time.sleep(0.02)

    def stop(self):
        self.needToExit = True
        self.thread.join()
        self.plumeEmitter.stopLed()

        print("stop Led")