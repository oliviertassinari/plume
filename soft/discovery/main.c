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
#include "test.h"

/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
#define DUTY 50
#define FREQ 5000


#define MY_CYCLE 100
static PWMConfig pwmcfg = {
  MY_CYCLE * FREQ,                 /* 10kHz PWM clock frequency.   */
  MY_CYCLE,                          /* PWM period 1S (in ticks).    */
  NULL,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  /* HW dependent part.*/
  0,
  0
};


/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
/*__attribute__((noreturn))
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(2000);
    palClearPad(GPIOD, GPIOD_LED3);
    chThdSleepMilliseconds(2000);
  }
}*/

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();


  /*
   * Initializes the PWM driver 4, routes the TIM4 outputs to the board LEDs.
   */
  pwmStart(&PWMD4, &pwmcfg);
  //palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));  /* Green.   */
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_ALTERNATE(2));  /* Blue.    */
  pwmEnableChannel(&PWMD4, 3, DUTY);
  //pwmEnableChannel(&PWMD4, 1, 50);
  /*
   * Creates the example thread.
   */
  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  palSetPad(GPIOD, 13);
  palClearPad(GPIOD, 12);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 2.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
