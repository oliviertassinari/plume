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
    global dequeX, dequeY, dequeZ, dequePhi, dequeTheta, dequePsi
    global lineX, lineY, lineZ, linePhi, lineTheta, linePsi

    dequeX = deque([0]*2000)
    dequeY = deque([0]*2000)
    dequeZ = deque([0]*2000)
    dequePhi = deque([0]*2000)
    dequeTheta = deque([0]*2000)
    dequePsi = deque([0]*2000)

    fig = plt.figure()
    fig.canvas.set_window_title('DOF')
    aPosition = plt.subplot(211)
    aOrientation = plt.subplot(212)

    aPosition.set_title('Positionnement en fonction du temps')
    aPosition.set_xlabel('Temps [U.A]')
    aPosition.set_ylabel('Position [m]')

    aOrientation.set_title('Orientation en fonction du temps')
    aOrientation.set_xlabel('Temps [U.A]')
    aOrientation.set_ylabel('Orientation [m]')

    aPosition.axis([0, 2000, 0, 4])
    lineX, = aPosition.plot(dequeX)
    lineY, = aPosition.plot(dequeY)
    lineZ, = aPosition.plot(dequeZ)
    aPosition.legend([lineX, lineY, lineZ], ["X", "Y", "Z"])

    aOrientation.axis([0, 2000, -180, 180])
    linePhi, = aOrientation.plot(dequePhi)
    lineTheta, = aOrientation.plot(dequeTheta)
    linePsi, = aOrientation.plot(dequePsi)
    aOrientation.legend([linePhi, lineTheta, linePsi], ["Phi", "Theta", "Psi"])

    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())

    plt.ion()
    plt.show()

def updatePlot():
    global fig
    global dequeX, dequeY, dequeZ, dequePhi, dequeTheta, dequePsi
    global lineX, lineY, lineZ, line3dX, line3dY, line3dZ, linePhi, lineTheta, linePsi
    lineX.set_ydata(dequeX)
    lineY.set_ydata(dequeY)
    lineZ.set_ydata(dequeZ)

    linePhi.set_ydata(dequePhi)
    lineTheta.set_ydata(dequeTheta)
    linePsi.set_ydata(dequePsi)

    fig.canvas.draw()
    fig.canvas.flush_events()

def _callback(type, MAC):
    global plumeApi
    global dequeX, dequeY, dequeZ, dequePhi, dequeTheta, dequePsi

    if type == TYPE_DOF:
        plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)
        position = np.zeros(3, dtype=np.double)
        orientation = np.zeros(3, dtype=np.double)
        plumeReceiver.get6DOFDeg(position, orientation)

        dequeX.appendleft(position[0])
        dequeX.pop()

        dequeY.appendleft(position[1])
        dequeY.pop()

        dequeZ.appendleft(position[2])
        dequeZ.pop()

        dequePhi.appendleft(orientation[0])
        dequePhi.pop()

        dequeTheta.appendleft(orientation[1])
        dequeTheta.pop()

        dequePsi.appendleft(orientation[2])
        dequePsi.pop()

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

    print("Start plot dof")

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