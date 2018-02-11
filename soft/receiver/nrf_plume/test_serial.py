
import serial
import sys

try:
    ser = serial.Serial("/dev/ttyACM0", int(sys.argv[1]), timeout=1)
except:
    print("enter bauderate")
#port.write(b'lolololol')
cpt = [0]
def _send(port, s):
    ii = cpt[0]
    for iii in s:
        port.write(bytearray(iii))
        cpt[0] += 1
        #print(bytearray(iii))
        if cpt[0] % 6 == 0:
            ack = port.read()
            print("ack")
    
def ssend (port, b):
    for bb in b:
        port.write(b)
        try:
            port.read()
        except:
            pass

def send (port, s):
    dat = bytearray()
    dat.append(0x7e)
    dat.append(len(s))
    [dat.append(ord(i)) for i in s]
    
    ssend(port, dat)
    #port.write(dat)
    print("0x%x" % len(s))

def send_reset (port):
    dat = bytearray()
    dat.append(0x13)
    dat.append(0x13)
    dat.append(0x13)
    port.write(dat)

import time
send_reset(ser)
for i in range(int(sys.argv[2])):
        #print(i)
    send(ser, "#Paquet %04d.!!!!!!!" % i)
    time.sleep(.01)
    print(i)


#port.close()
