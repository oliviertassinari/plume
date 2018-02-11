#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import time
import signal
from threading import Lock

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")
sys.path.insert(0, path + "/../led")

from plume_api import *
from led import *

lock = Lock()
count = dict()

def getCount(MAC):
    lock.acquire()

    if not MAC in count:
        count[MAC] = 0

    value = count[MAC]

    lock.release()

    return value

def updateCount(MAC):
    lock.acquire()

    if not MAC in count:
        count[MAC] = 0

    count[MAC] += 1

    lock.release()

def printMonitoring():
    global t0

    os.system('clear')

    print("Monitoring")

    t1 = time.time()

    for i in range(plumeApi.getPlumeReceiversSize()):
        print("---")
        plumeReceiver = plumeApi.getPlumeReceiversAt(i)
        print("index : " + str(i))
        print("address : " + str(plumeReceiver.getMACHex()))
        print("state : " + str(plumeReceiver.getState()))
        #print("battery : " + str(plumeReceiver.getBattery()))
        position = np.zeros(3, dtype=np.double)
        orientation = np.zeros(3, dtype=np.double)
        plumeReceiver.get6DOFDeg(position, orientation)
        print("position : " + np.array_str(position))
        #print("orientation : " + np.array_str(orientation))
        print("frequency : " + str(round(getCount(plumeReceiver.MAC)/(t1-t0))) + " Hz")

        count[plumeReceiver.MAC] = 0

    t0 = time.time()

def _callback(type, MAC):
    if type == TYPE_DOF:
        updateCount(MAC)

callback = callback_t(_callback)

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0Led

    print("You pressed Ctrl+C - or killed me with -2")

    plumeEmitter0Led.stop()
    del plumeApi

    sys.exit(0)

def init():
    global plumeApi, t0, plumeEmitter0Led

    t0 = time.time()

    print("Start monitoring")

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)
    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(50)
    plumeApi.setCallback(callback)
    plumeApi.startEmitters()

    """
    # Wait one receiver
    plumeApi.scanDevice()

    while plumeApi.getPlumeReceiversSize() < 1:
        continue

    plumeApi.getPlumeReceiversAt(0).start();
    """

    plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    plumeApi.startReceiver("f59f2f10b721") # Start a specific receiver to debug
    #plumeApi.startReceiver("e3794ec582f5") # Start a specific receiver to debug
    #plumeApi.startReceiver("f281c1d4755e") # Start a specific receiver to debug

    plumeEmitter0Led.setState(1)

    while True:
        printMonitoring()
        time.sleep(1)

init()