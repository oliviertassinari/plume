#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import time
import signal
import matplotlib.pyplot as plt
import matplotlib.legend
import numpy as np
from collections import deque

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")
sys.path.insert(0, path + "/../led")

from plume_api import *
from led import *

def initPlot():
    global fig
    global dequeExRx, dequeEyRx, dequeEzRx, dequeExRy, dequeEyRy, dequeEzRy, dequeExRz, dequeEyRz, dequeEzRz
    global lineExRx, lineEyRx, lineEzRx, lineExRy, lineEyRy, lineEzRy, lineExRz, lineEyRz, lineEzRz

    dequeExRx = deque([1]*2000)
    dequeEyRx = deque([1]*2000)
    dequeEzRx = deque([1]*2000)

    dequeExRy = deque([1]*2000)
    dequeEyRy = deque([1]*2000)
    dequeEzRy = deque([1]*2000)

    dequeExRz = deque([1]*2000)
    dequeEyRz = deque([1]*2000)
    dequeEzRz = deque([1]*2000)

    fig = plt.figure()
    fig.canvas.set_window_title('Voltages')
    aRx = fig.add_subplot(1, 3, 1)
    aRy = fig.add_subplot(1, 3, 2)
    aRz = fig.add_subplot(1, 3, 3)
   
    aRx.set_title(u'Bobine réceptrice X')
    
    aRx.axis([0, 2000, 10e2, 10e8])
    aRx.set_yscale('log')
    lineExRx, = aRx.plot(dequeExRx)
    lineEyRx, = aRx.plot(dequeEyRx)
    lineEzRx, = aRx.plot(dequeEzRx)
    aRx.legend([lineExRx, lineEyRx, lineEzRx], ["ExRx", "EyRx", "EzRx"])

    aRy.set_title(u'Bobine réceptrice Y')
    
    aRy.set_yscale('log')
    aRy.axis([0, 2000, 10e2, 10e8])
    lineExRy, = aRy.plot(dequeExRy)
    lineEyRy, = aRy.plot(dequeEyRy)
    lineEzRy, = aRy.plot(dequeEzRy)
    aRy.legend([lineExRy, lineEyRy, lineEzRy], ["ExRy", "EyRy", "EzRy"])

    aRz.set_title(u'Bobine réceptrice Z')
    
    aRz.axis([0, 2000, 10e2, 10e8])
    aRz.set_yscale('log')
    lineExRz, = aRz.plot(dequeExRz)
    lineEyRz, = aRz.plot(dequeEyRz)
    lineEzRz, = aRz.plot(dequeEzRz)
    aRz.legend([lineExRz, lineEyRz, lineEzRz], ["ExRz", "EyRz", "EzRz"])

    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())

    plt.ion()
    plt.show()

def updatePlot():
    global fig
    global dequeExRx, dequeEyRx, dequeEzRx, dequeExRy, dequeEyRy, dequeEzRy, dequeExRz, dequeEyRz, dequeEzRz
    global lineExRx, lineEyRx, lineEzRx, lineExRy, lineEyRy, lineEzRy, lineExRz, lineEyRz, lineEzRz

    lineExRx.set_ydata(dequeExRx)
    lineEyRx.set_ydata(dequeEyRx)
    lineEzRx.set_ydata(dequeEzRx)

    lineExRy.set_ydata(dequeExRy)
    lineEyRy.set_ydata(dequeEyRy)
    lineEzRy.set_ydata(dequeEzRy)

    lineExRz.set_ydata(dequeExRz)
    lineEyRz.set_ydata(dequeEyRz)
    lineEzRz.set_ydata(dequeEzRz)

    fig.canvas.draw()
    fig.canvas.flush_events()

def _callback(type, MAC):
    global plumeApi
    global dequeExRx, dequeEyRx, dequeEzRx, dequeExRy, dequeEyRy, dequeEzRy, dequeExRz, dequeEyRz, dequeEzRz

    if type == TYPE_DOF:
        plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)
        voltages = np.zeros(9, dtype=np.double)
        plumeReceiver.getVoltages(voltages)

        dequeExRx.appendleft(abs(voltages[0]))
        dequeExRx.pop()

        dequeExRy.appendleft(abs(voltages[1]))
        dequeExRy.pop()

        dequeExRz.appendleft(abs(voltages[2]))
        dequeExRz.pop()

        dequeEyRx.appendleft(abs(voltages[3]))
        dequeEyRx.pop()

        dequeEyRy.appendleft(abs(voltages[4]))
        dequeEyRy.pop()

        dequeEyRz.appendleft(abs(voltages[5]))
        dequeEyRz.pop()

        dequeEzRx.appendleft(abs(voltages[6]))
        dequeEzRx.pop()

        dequeEzRy.appendleft(abs(voltages[7]))
        dequeEzRy.pop()

        dequeEzRz.appendleft(abs(voltages[8]))
        dequeEzRz.pop()

callback = callback_t(_callback)

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0Led, needToStop

    print("You pressed Ctrl+C - or killed me with -2")

    needToStop = True
    plumeEmitter0Led.stop()
    del plumeApi

    sys.exit(0)

def main():
    global plumeApi, plumeEmitter0Led, needToStop

    print("Start plot voltages")

    initPlot()

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

    needToStop = False

    while not needToStop:
        updatePlot()

if __name__ == "__main__":
    main()