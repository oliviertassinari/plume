import sys, os
import bge
from bge import logic
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/computer/plume_api/python')

from plume_api import *

def main():
    # To get the controller thats running this python script:
    cont = logic.getCurrentController()
    # To get the game object this controller is on:
    obj = cont.owner

    plumeReceiver = logic.plumeApi.getPlumeReceiverFromMACHex("f59f2f10b721")
    position = np.zeros(3, dtype=np.double)
    orientation = np.zeros(3, dtype=np.double)
    #plumeReceiver.get6DOFDeg(position, orientation)
    #print(orientation)
    #print(position)
    plumeReceiver.get6DOFRad(position, orientation)

    #obj.localOrientation = orientation
    obj.localPosition = position

main()