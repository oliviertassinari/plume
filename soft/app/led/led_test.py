#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import time
import signal

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/../../computer/plume_api/python')

from plume_api import *

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0

    print("You pressed Ctrl+C - or killed me with -2")

    plumeEmitter0.stopLed()
    del plumeApi

    sys.exit(0)

def init():
    global plumeApi, plumeEmitter0

    print("test ok")

    plumeApi = PlumeApi("../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0.startLed()

    octant = np.zeros([2, 2, 2], dtype=('u1,u1,u1'))

    for x in range(2):
      for y in range(2):
        for z in range(2):
          octant[x, y, z][0] = 255
          octant[x, y, z][1] = 0
          octant[x, y, z][2] = 0

    plumeEmitter0.setLed(octant)

    toAdd = 2
    BC = 0
    color = 0

    while True:
        BC += toAdd

        if BC > 201:
            BC = 200
            toAdd = -2
        elif BC < 0:
            BC = 1
            toAdd = 2
            color += 1

            if color == 3:
                color = 0

            for x in range(2):
              for y in range(2):
                for z in range(2):
                  octant[x, y, z][0] = 255 if color == 0 else 0 # Red
                  octant[x, y, z][1] = 255 if color == 1 else 0 # Green
                  octant[x, y, z][2] = 255 if color == 2 else 0 # Blue

            plumeEmitter0.setLed(octant)

        plumeEmitter0.setLedBrightless(BC)
        time.sleep(0.02)

init()