import os
from ctypes import *
import ctypes
import binascii

lib = cdll.LoadLibrary(os.path.dirname(__file__)+ '/../../calculator/build/plume_api.so')

callback_t = CFUNCTYPE(None, c_int, c_char_p)

STATE_UNKNOWN = 0
STATE_CONNECT = 1
STATE_DISCONNECT = 2
STATE_TRY = 3

TYPE_DOF = 0
TYPE_BATTERY = 1
TYPE_STATE = 2
TYPE_BUTTON = 3

ENABLE_UNKNOWN = 0
ENABLE_ON = 0xE
ENABLE_OFF = 0xF
ENABLE_STANDBY = 0x10

class PlumeApi(object):
    def __init__(self, jsonFile):
        lib.PlumeApiNew.restype = c_void_p
        self.obj = lib.PlumeApiNew(jsonFile.encode('ascii'))

    def setCallback(self, callback):
        lib.PlumeApiSetCallback.argtypes = [c_void_p, callback_t]
        lib.PlumeApiSetCallback.restype = None
        lib.PlumeApiSetCallback(self.obj, callback)

    def addEmitter(self, MAC, portname):
        lib.PlumeApiAddEmitter.restype = c_void_p
        plumeEmitter = lib.PlumeApiAddEmitter(self.obj, MAC.encode('ascii'), portname.encode('ascii'))
        return PlumeEmitter(plumeEmitter)

    def startEmitters(self):
        lib.PlumeApiStartEmitters(self.obj)

    def stopEmitters(self):
        lib.PlumeApiStopEmitters(self.obj)

    def startReceivers(self):
        lib.PlumeApiStartReceivers(self.obj)

    def startReceiver(self, MAC):
        return lib.PlumeApiStartReceiver(self.obj, binascii.a2b_hex(MAC))

    def stopReceivers(self):
        lib.PlumeApiStopReceivers(self.obj)

    def end(self):
        lib.PlumeApiEnd(self.obj)

    def scanDevice(self):
        lib.PlumeApiScanDevice(self.obj)

    def setFrequency(self, frequency):
       lib.PlumeApiSetFrequency(self.obj, frequency)

    def getPlumeReceiversSize(self):
        return lib.PlumeReceiversSize(self.obj)

    def getPlumeReceiversAt(self, i):
        lib.PlumeReceiversAt.restype = c_char_p
        return PlumeReceiver(self.obj, lib.PlumeReceiversAt(self.obj, i))

    def getPlumeReceiverFromMACHex(self, MAC):
        return PlumeReceiver(self.obj, binascii.a2b_hex(MAC))

    def getPlumeReceiverFromMAC(self, MAC):
        return PlumeReceiver(self.obj, MAC)

    def __del__(self):
        lib.PlumeApiDelete(self.obj)

        print("del PlumeApi")

class PlumeEmitter(object):
    def __init__(self, obj):
        self.obj = obj

    def startLed(self):
        lib.PlumeEmitterStartLed(self.obj)

    def stopLed(self):
        lib.PlumeEmitterStopLed(self.obj)

    def setLed(self, octant):
        lib.PlumeEmitterSetLed(self.obj, ctypes.c_void_p(octant.ctypes.data))

    def setLedBrightless(self, BC):
        lib.PlumeEmitterSetLedBrightless(self.obj, BC)

class PlumeReceiver(object):
    def __init__(self, plumeApi, MAC):
        self.plumeApi = plumeApi
        self.MAC = MAC

    def getMACHex(self):
        """
        Return the MAC address in Hex for a print.
        """

        lst = []
        for i in range(6):
            hv = hex(ord(self.MAC[i])).replace('0x', '')
            if len(hv) == 1:
                hv = '0'+hv
            lst.append(hv)

        return reduce(lambda x,y:x+y, lst)

    def start(self):
       lib.PlumeReceiverStart(self.plumeApi, self.MAC);

    def get6DOFDeg(self, position, orientation):
        lib.PlumeReceiverGet6DOFDeg(self.plumeApi, self.MAC, ctypes.c_void_p(position.ctypes.data), ctypes.c_void_p(orientation.ctypes.data))

    def get6DOFRad(self, position, orientation):
        lib.PlumeReceiverGet6DOFRad(self.plumeApi, self.MAC, ctypes.c_void_p(position.ctypes.data), ctypes.c_void_p(orientation.ctypes.data))

    def setState(self, state):
       lib.PlumeReceiverSetState(self.plumeApi, self.MAC, state);

    def getBattery(self):
        lib.PlumerReceiverGetBattery.restype = c_uint
        return lib.PlumerReceiverGetBattery(self.plumeApi, self.MAC)

    def getState(self):
       lib.PlumerReceiverGetState.restype = c_uint
       return lib.PlumerReceiverGetState(self.plumeApi, self.MAC)

    def getVoltages(self, voltages):
       return lib.PlumerReceiverGetVoltages(self.plumeApi, self.MAC, ctypes.c_void_p(voltages.ctypes.data))

    def getButton(self):
        lib.PlumerReceiverGetBattery.restype = c_uint
        return lib.PlumerReceiverGetButton(self.plumeApi, self.MAC)