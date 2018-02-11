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
    global plumeApi, needToStop

    print("You pressed Ctrl+C - or killed me with -2")

    needToStop = True
    del plumeApi

    sys.exit(0)

def _callback(type, MAC):
    global plumeApi, device

    if type == TYPE_BUTTON:
        plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)
        button = plumeReceiver.getButton()
        
        print("button pressed")

        if button & 0x1:
            device.emit_click(uinput.KEY_PAGEDOWN)

        if (button & 0x2) >> 1:
            device.emit_click(uinput.KEY_PAGEUP)


callback = callback_t(_callback)

def init():
    global plumeApi, device, needToStop

    print("Start keyboard")

    events = (
        uinput.KEY_PAGEDOWN,
        uinput.KEY_PAGEUP,
    )

    device = uinput.Device(events)

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")

    plumeApi.setFrequency(50)
    plumeApi.setCallback(callback)
    """
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
        time.sleep(0.01)

    plumeApi.startEmitters()
    plumeApi.getPlumeReceiversAt(0).start();
    """
    #plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    #plumeApi.startReceiver("f59f2f10b721") # Start a specific receiver to debug
    #plumeApi.startReceiver("e3794ec582f5") # Start a specific receiver to debug
    plumeApi.startReceiver("f281c1d4755e") # Start a specific receiver to debug

    needToStop = False

    while not needToStop:
        time.sleep(1)

init()