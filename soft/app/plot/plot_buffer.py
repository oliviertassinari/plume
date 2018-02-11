#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os
import numpy as np
import time
import signal
import matplotlib.pyplot as plt
import matplotlib.legend
import numpy as np
import math
from array import array
from random import gauss
from collections import deque

path = os.path.dirname(os.path.abspath(__file__))

sys.path.insert(0, path + "/../../computer/plume_api/python")
sys.path.insert(0, path + "/../led")

from plume_api import *
from led import *

def initPlot():
    global fig
    global arrayRx, arrayRy, arrayRz
    global lineRx, lineRy, lineRz

    arrayRx = array('d',[1]*1920)
    arrayRy = array('d',[1]*1920)
    arrayRz = array('d',[1]*1920)

    fig = plt.figure()
    fig.canvas.set_window_title('Buffer')
    aRx = fig.add_subplot(1, 3, 1)
    aRy = fig.add_subplot(1, 3, 2)
    aRz = fig.add_subplot(1, 3, 3)

    aRx.set_title(u'Bobine réceptrice X')
    aRx.set_xlabel('Temps [U.A.]')
    aRx.set_ylabel('Tension [U.A.]')
    aRx.axis([0, 200, -5e7, 5e7])
    lineRx, = aRx.plot(arrayRx)
    aRx.legend([lineRx], ["Rx"])

    aRy.set_title(u'Bobine réceptrice Y')
    aRy.set_xlabel('Temps [U.A.]')
    aRy.axis([0, 200, -5e7, 5e7])
    lineRy, = aRy.plot(arrayRy)
    aRy.legend([lineRy], ["Ry"])

    aRz.set_title(u'Bobine réceptrice Z')
    aRz.set_xlabel('Temps [U.A.]')
    aRz.axis([0, 200, -5e7, 5e7])
    lineRz, = aRz.plot(arrayRz)
    aRz.legend([lineRz], ["Rz"])

    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())

    plt.ion()
    plt.show()

def updatePlot():
    global fig
    global arrayRx, arrayRy, arrayRz
    global lineRx, lineRy, lineRz
    global mutex

    lineRx.set_ydata(arrayRx)
    lineRy.set_ydata(arrayRy)
    lineRz.set_ydata(arrayRz)

    fig.canvas.draw()
    fig.canvas.flush_events()

def _callback(type, MAC):
    global plumeApi
    global arrayRx, arrayRy, arrayRz, arrayExp, arrayCos, mutex, arrayGauss

    if type == TYPE_DOF:
        plumeReceiver = plumeApi.getPlumeReceiverFromMAC(MAC)
        voltages = np.zeros(9, dtype=np.double)
        plumeReceiver.getVoltages(voltages)

        for i in range(0,3):
            for j in range(0,50):

                u=arrayExp[j]
                g=gauss(0.0,1e6)

                a=arrayCos[j]*abs(voltages[3*i])
                if i>0:
                    b=arrayCos[j]*abs(voltages[3*i-3])
                else:
                    b=0
                arrayRx[50*i+j]=((1-u)*a+u*b+g)

                a=arrayCos[j]*abs(voltages[3*i+1])
                if i>0:
                    b=arrayCos[j]*abs(voltages[3*i-2])
                else:
                    b=0
                arrayRy[50*i+j]=((1-u)*a+u*b+g)

                a=arrayCos[j]*abs(voltages[3*i+2])
                if i>0:
                    b=arrayCos[j]*abs(voltages[3*i-1])
                else:
                    b=0
                arrayRz[50*i+j]=((1-u)*a+u*b+g)
        for j in range (0,50):

            u=arrayExp[j]
            g=gauss(0.0,1e6)

            b=arrayCos[j]*abs(voltages[6])
            arrayRx[150+j]=(u*b+g)

            b=arrayCos[j]*abs(voltages[7])
            arrayRy[150+j]=u*b+g

            b=arrayCos[j]*abs(voltages[8])
            arrayRz[150+j]=u*b+g

callback = callback_t(_callback)

def signalHandler(signal, frame):
    global plumeApi, plumeEmitter0Led, needToStop

    print("You pressed Ctrl+C - or killed me with -2")

    needToStop = True
    plumeEmitter0Led.stop()
    del plumeApi

    sys.exit(0)

def main():
    global plumeApi, plumeEmitter0Led, needToStop, arrayExp, arrayCos, arrayGauss, mutex

    print("Start plot buffer")

    arrayExp = array('d',[])
    arrayCos = array('d',[])
    arrayGauss = array('d',[])

    for i in range(0,50):
        arrayExp.append(math.exp(-i/5))
        arrayCos.append(math.cos(2*i*math.pi*1.8))
        arrayGauss.append(gauss(0.0,1e6))

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