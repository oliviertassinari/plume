#include "serial_shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "custom_commands.h"

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


/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WA_SIZE(4096)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state time\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
                (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                states[tp->p_state], (uint32_t)tp->p_time);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}


static void cmd_write(BaseSequentialStream *chp, int argc, char *argv[]) {
    static uint8_t buf[] =
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: write\r\n");
        return;
    }

    while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
        chSequentialStreamWrite(&SD2, buf, sizeof buf - 1);
    }
    chprintf(chp, "\r\n\nstopped\r\n");
}

static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"spiread", cmd_spiread},
    {"set_amp", cmd_set_amp},
    {"get_amp", cmd_get_amp},
    {"reset_codecs", cmd_reset_codecs},
    {"spiwrite", cmd_spiwrite},
    {"write", cmd_write},
    {"codec_write", cmd_codecwrite},
    {"codec_read", cmd_codecread},
    {"start_meas",cmd_startmeasure},
    {"stop_meas",cmd_stopmeasure}
};

static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&SD2,
    commands
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

    Thread *shelltp = NULL;
    (void)arg;

    /*
     * Normal main() thread activity, in this demo it does nothing except
     * sleeping in a loop and check the button state.
     */
    while (TRUE) {
        if (!shelltp)
            shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO+1);
        else if (chThdTerminated(shelltp)) {
            chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
            shelltp = NULL;           /* Triggers spawning of a new shell.        */
        }
        chThdSleepMilliseconds(1000);
    }
    return 0;
}


static SerialConfig ser_cfg = {
    115200,
    0,
    0,
    0,
};


/*
 * Application entry point.
 */
void serial_shell_init(void) {
    /*
     * Initializes a serial driver.
     */
    sdStart(&SD2, &ser_cfg);

    /*
     * Shell manager initialization.
     */
    shellInit();

    /*
     * Creates the blinker thread.
     */
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

}
