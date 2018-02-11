#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
import time
import sys
import os
import signal

print("Start demo")

# Don't forward signals.
def preexec():
    os.setpgrp()

def signalHandler(signal1, frame):
    global proc1, proc2

    if proc2:
        os.kill(proc2.pid, signal.SIGINT)
        proc2.wait()

    if proc1:
        os.kill(proc1.pid, signal.SIGINT)
        proc1.wait()

    print("Close demo")

    sys.exit(0)

def main():
    global proc1, proc2

    proc1 = 0
    proc2 = 0

    signal.signal(signal.SIGINT, signalHandler)

    proc1 = subprocess.Popen(["iced", "../../soft/computer/ble_interface/main.coffee"], preexec_fn = preexec)
    time.sleep(3)

    proc2 = subprocess.Popen(["python", "joystick/keyboard.py"], preexec_fn = preexec)
    time.sleep(3)

    raw_input("Press Enter to quit...")

    os.kill(proc2.pid, signal.SIGINT)
    proc2.wait()

    os.kill(proc1.pid, signal.SIGINT)
    proc1.wait()

    sys.exit(0)

main()