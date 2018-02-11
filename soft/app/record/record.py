#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import time
import signal
import Tkinter, tkFileDialog
import csv

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")

from plume_api import *

def _callback(type, MAC):
    global plumeApi, file

    if type == TYPE_DOF:
        plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)
        voltages = np.zeros(9, dtype=np.double)
        plumeReceiver.getVoltages(voltages)

        writer = csv.writer(file, delimiter=' ')
        writer.writerows([voltages])

        print("write")

callback = callback_t(_callback)

def signalHandler(signal, frame):
    global plumeApi, needToStop, file

    print("You pressed Ctrl+C - or killed me with -2")

    file.close()
    needToStop = True
    del plumeApi

    sys.exit(0)

def main():
    global plumeApi, needToStop, file

    print("Start record")

    root = Tkinter.Tk()
    root.withdraw()
    filename = tkFileDialog.asksaveasfilename()
    file = open(filename, "wb")

    plumeApi = PlumeApi(path + "/../../computer/calculator/calibration.json")

    signal.signal(signal.SIGINT, signalHandler)

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")

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

    needToStop = False

    while not needToStop:
        time.sleep(1)

if __name__ == "__main__":
    main()