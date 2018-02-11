#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import uinput

def main():
    events = (
        uinput.ABS_X,
        uinput.ABS_Y,
        uinput.BTN_LEFT,
        uinput.BTN_RIGHT,
    )

    device = uinput.Device(events)

    for i in range(150):
        # syn=False to emit an "atomic" (5, 5) event.
        device.emit(uinput.ABS_X, 5*i, syn=False)
        device.emit(uinput.ABS_Y, 5*i)

        time.sleep(0.01)

main()