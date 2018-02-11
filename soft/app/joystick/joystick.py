#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import uinput
import time
import signal

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")
sys.path.insert(0, path + "/../led")

from plume_api import *
from led import *

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0Led

    print("You pressed Ctrl+C - or killed me with -2")

    plumeEmitter0Led.stop()
    del plumeApi

    sys.exit(0)

def init():
    global plumeApi, plumeEmitter0Led

    print("Start joystick")

    events = (
        uinput.ABS_X,
        uinput.ABS_Y,
        uinput.ABS_Z,
        uinput.ABS_RX,
        uinput.ABS_RY,
        uinput.ABS_RZ
    )

    device = uinput.Device(events)

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)
    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(100)
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
        time.sleep(0.01)

    plumeApi.startEmitters()
    plumeApi.getPlumeReceiversAt(0).start();

    plumeEmitter0Led.setState(1)

    while True:
        plumeReceiver = plumeApi.getPlumeReceiversAt(0)
        position = np.zeros(3, dtype=np.double)
        orientation = np.zeros(3, dtype=np.double)
        plumeReceiver.get6DOF(position, orientation)
        print(position)
        print(orientation)

        # syn=False to emit an "atomic" event.
        device.emit(uinput.ABS_X, int(position[0]*100), syn=False)
        device.emit(uinput.ABS_Y, int(position[1]*100), syn=False)
        device.emit(uinput.ABS_Z, int(position[2]*100), syn=False)

        device.emit(uinput.ABS_RX, int(orientation[0]), syn=False)
        device.emit(uinput.ABS_RY, int(orientation[1]), syn=False)
        device.emit(uinput.ABS_RZ, int(orientation[2]))

        time.sleep(0.01)

init()