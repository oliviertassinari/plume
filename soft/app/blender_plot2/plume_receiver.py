import sys, os
import bge
from bge import logic
import numpy as np
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/computer/plume_api/python')
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/app/led')

from plume_api import *
from led import *

class End(object):
    def __init__(self, plumeEmitterLed):
        self.plumeEmitterLed = plumeEmitterLed

    def __del__(self):
        self.plumeEmitterLed.stop()

def init():
    print("Plume receiver start")

    plumeApi = PlumeApi("../computer/calculator/calibration.json")

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)

    plumeEmitter0Led = plumeEmitter0Led

    logic.end = End(plumeEmitter0Led)
    logic.plumeApi = plumeApi

    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(50)
    plumeApi.startEmitters()
    """
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
      time.sleep(0.01)

    plumeApi.getPlumeReceiversAt(0).start();
    """

    plumeApi.startReceiver("f59f2f10b721") # Start a specific receiver to debug
    plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    #plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    #plumeApi.startReceiver("f281c1d4755e") # Start a specific receiver to debug

    plumeEmitter0Led.setState(1)

    print("init done")

if not hasattr(logic, 'plumeApi'):
    init()