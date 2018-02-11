#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import numpy as np
import time
import signal

from plume_api import *

def _callback(type, MAC):
    global plumeApi

    plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)

    if type == TYPE_DOF:
        print("new DOF " + str(plumeReceiver.getMACHex()))
        position = np.zeros(3, dtype=np.double)
        orientation = np.zeros(3, dtype=np.double)
        plumeReceiver.get6DOFDeg(position, orientation)
        print(position)
        print(orientation)
    elif type == TYPE_BATTERY:
        print("new battery " + str(plumeReceiver.getMACHex()) + " " + str(plumeReceiver.getBattery()))
    elif type == TYPE_STATE:
        print("new state " + str(plumeReceiver.getMACHex()) + " " + str(plumeReceiver.getState()))

callback = callback_t(_callback)

def signalHandler(signal, frame):
    global plumeApi

    print("You pressed Ctrl+C - or killed me with -2")

    del plumeApi

    sys.exit(0)

def init():
    global plumeApi

    print("test plume API")

    plumeApi = PlumeApi("../../calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")

    plumeApi.setFrequency(50)
    plumeApi.setCallback(callback)
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
        time.sleep(0.01)

    plumeApi.startEmitters()
    plumeApi.getPlumeReceiversAt(0).start();

    while True:
        for i in range(plumeApi.getPlumeReceiversSize()):
            plumeReceiver = plumeApi.getPlumeReceiversAt(i)
            print(plumeReceiver.getMACHex())
            position = np.zeros(3, dtype=np.double)
            orientation = np.zeros(3, dtype=np.double)
            plumeReceiver.get6DOFDeg(position, orientation)
            print(position)

        time.sleep(0.5)

init()