#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import uinput
import time
import signal
import struct

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

def _callback(type, MAC):
    global plumeApi, device

    if type == TYPE_BUTTON:
        plumeReceiver = plumeApi.getPlumeReceiverFromMACHex("f59f2f10b721")
        button = plumeReceiver.getButton()

        if button & 0x1:
            device.emit_click(uinput.BTN_LEFT)

        if (button & 0x2) >> 1:
            device.emit_click(uinput.BTN_RIGHT)


callback = callback_t(_callback)

def init():
    global plumeApi, plumeEmitter0Led, device

    print("Start mouse")

    events = (
        uinput.ABS_X,
        uinput.ABS_Y,
        uinput.BTN_LEFT,
        uinput.BTN_RIGHT,
    )

    device = uinput.Device(events)

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)
    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(50)
    plumeApi.setCallback(callback)
    plumeApi.startEmitters()
    """
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
        continue

    plumeApi.getPlumeReceiversAt(0).start();
    """
    plumeApi.startReceiver("f59f2f10b721") # Start a specific receiver to debug
    #plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    #plumeApi.startReceiver("e3794ec582f5") # Start a specific receiver to debug
    #plumeApi.startReceiver("f281c1d4755e") # Start a specific receiver to debug

    plumeEmitter0Led.setState(1)

    while True:
        plumeReceiver = plumeApi.getPlumeReceiverFromMACHex("f59f2f10b721")
        position = np.zeros(3, dtype=np.double)
        orientation = np.zeros(3, dtype=np.double)
        plumeReceiver.get6DOFDeg(position, orientation)
        print(position)

        device.emit(uinput.ABS_X, int(position[0])*800)
        device.emit(uinput.ABS_Y, int(position[1])*800)

        time.sleep(0.02)

init()