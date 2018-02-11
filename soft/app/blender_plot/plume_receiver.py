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
    #plumeApi.startReceiver("e3794ec582f5") # Start a specific receiver to debug
    #plumeApi.startReceiver("e6b3a059065c") # Start a specific receiver to debug
    #plumeApi.startReceiver("f281c1d4755e") # Start a specific receiver to debug

    plumeEmitter0Led.setState(1)

    print("init done")

def main():
    # To get the controller thats running this python script:
    cont = logic.getCurrentController()
    # To get the game object this controller is on:
    obj = cont.owner

    plumeReceiver = PlumeReceiver(logic.plumeApi.obj, binascii.a2b_hex("f59f2f10b721"))
    position = np.zeros(3, dtype=np.double)
    orientation = np.zeros(3, dtype=np.double)
    #plumeReceiver.get6DOFDeg(position, orientation)
    #print(orientation)
    #print(position)
    plumeReceiver.get6DOFRad(position, orientation)

    #obj.localOrientation = orientation
    obj.localPosition = position

if not hasattr(logic, 'plumeApi'):
    init()

main()