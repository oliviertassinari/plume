#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
import
'''

import sys, os
import numpy as np
import time
import signal

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")
sys.path.insert(0, path + "/../led")

from plume_api import *
from led import *

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0Led, needToStop

    print("You pressed Ctrl+C - or killed me with -2")

    needToStop = True
    plumeEmitter0Led.stop()
    del plumeApi

    sys.exit(0)

def init():
    global plumeApi, plumeEmitter0Led, needToStop

    print("Start led animation")

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)
    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(50)
    plumeApi.startEmitters()

    needToStop = False

    while not needToStop:
        time.sleep(1)

if __name__ == "__main__":
    init()
