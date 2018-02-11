import sys, os
import bge
from bge import logic
import numpy as np
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/../computer/plume_api/python')
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/led')

from plume_api import *
from led import *

class End(object):
    def __init__(self, plumeEmitterLed):
        self.plumeEmitterLed = plumeEmitterLed

    def __del__(self):
        self.plumeEmitterLed.stop()

def init():
    print("Plume receiver start")

    plumeApi = PlumeApi("../../computer/calculator/calibration.json")

    plumeEmitter0 = plumeApi.addEmitter("1", "/dev/ttyACM0")
    plumeEmitter0Led = PlumeEmitterLed(plumeEmitter0)

    plumeEmitter0Led = plumeEmitter0Led

    logic.end = End(plumeEmitter0Led)
    logic.plumeApi = plumeApi

    plumeEmitter0Led.setState(0)

    plumeApi.setFrequency(100)
    plumeApi.scanDevice()

    # Wait one receiver
    while plumeApi.getPlumeReceiversSize() < 1:
      time.sleep(0.01)

    plumeApi.startEmitters()
    plumeApi.getPlumeReceiversAt(0).start();

    plumeEmitter0Led.setState(1)

    print("init done")

def main():
    # To get the controller thats running this python script:
    cont = logic.getCurrentController()
    # To get the game object this controller is on:
    obj = cont.owner

    plumeReceiver = logic.plumeApi.getPlumeReceiversAt(0)
    position = np.zeros(3, dtype=np.double)
    orientation = np.zeros(3, dtype=np.double)
    #plumeReceiver.get6DOFDeg(position, orientation)
    #print(orientation)
    plumeReceiver.get6DOFRad(position, orientation)

    obj.localOrientation = orientation
    obj.localPosition = position

if not hasattr(logic, 'plumeApi'):
    init()

main()