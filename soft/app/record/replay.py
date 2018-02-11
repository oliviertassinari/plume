#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import pika
import time
import signal
import Tkinter, tkFileDialog
import csv
import struct
import binascii

def signalHandler(signal, frame):
    global needToStop, file

    print("You pressed Ctrl+C - or killed me with -2")

    file.close()
    needToStop = True

    sys.exit(0)

def main():
    global needToStop, file

    print("Start replay")

    signal.signal(signal.SIGINT, signalHandler)

    credentials = pika.PlainCredentials('guest', 'guest')
    connection = pika.BlockingConnection(pika.ConnectionParameters('localhost', 5672, '/', credentials))
    channel = connection.channel(1)
    channel.queue_declare(queue="plume ble to calculator", auto_delete=True)
    channel.queue_declare(queue="plume calculator to ble", auto_delete=True)

    root = Tkinter.Tk()
    root.withdraw()
    filename = tkFileDialog.askopenfilename()
    file = open(filename, "rb")
    reader = csv.reader(file, delimiter=' ') 

    needToStop = False

    while not needToStop:
        file.seek(0)

        print("replay " + filename)

        data = "s"
        data += binascii.a2b_hex("f59f2f10b721")
        data += chr(0x01)
        channel.basic_publish('', routing_key="plume ble to calculator", body=data)

        for i, line in enumerate(reader):
            data = "v"
            data += binascii.a2b_hex("f59f2f10b721")
            data += chr(0)
            data += chr(0)
            sign1 = 0
            sign2 = 0

            for i in range(9):
                f = int(float(line[i]))
                if f < 0:
                    fnorm = -f
                    sign = 1
                else:
                    fnorm = f
                    sign = 0

                data += struct.pack("!L", fnorm)

                if i < 8:
                    sign1 |= (sign << i)
                else:
                    sign2 |= sign

            dataList = list(data)
            dataList[7] = chr(sign2)
            dataList[8] = chr(sign1)
            data = ''.join(dataList)

            channel.basic_publish('', routing_key="plume ble to calculator", body=data)
            time.sleep(0.02)

        for i in range(100):
            data = "v"
            data += binascii.a2b_hex("f59f2f10b721")
            data += chr(0)
            data += chr(0)

            for i in range(9):
                data += struct.pack("!L", 0)

            channel.basic_publish('', routing_key="plume ble to calculator", body=data)
            time.sleep(0.02)

    file.close()
    channel.close()

if __name__ == "__main__":
    main()