/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "hbridge.h"
#include "usb_slave.h"
#include "led.h"

// Red LED blinker thread, times are in milliseconds.
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {
    (void)arg;

    chRegSetThreadName("blinker");
    while (TRUE) {
        palClearPad(GPIOB, GPIOB_LED);
        chThdSleepMilliseconds(250);
        palSetPad(GPIOB, GPIOB_LED);
        chThdSleepMilliseconds(250);
    }

    return 0;
}

int main(void){
    /*
    * System initializations.
    * - HAL initialization, this also initializes the configured device drivers
    *   and performs the board-specific initializations.
    * - Kernel initialization, the main() function becomes a thread and the
    *   RTOS is active.
    */
    halInit();
    chSysInit();

    // Creates the blinker thread.
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    usbSlaveStart();
    //hbridgeStart();
    //ledStart();

    while (TRUE) {
        chThdSleepMilliseconds(500);
    }

    return 0;
}