#include "ch.h"
#include "hal.h"
#include "leds.h"

/*
 * Function to set intensity on the led
 */
static void led_on(PWMDriver *pwd) {
  (void)pwd;
  palSetPad(GPIOA, GPIOA_LED_STM);
}

/*
 * Function to clear off the led
 */
static void led_off(PWMDriver *pwd) {
  (void)pwd;
  palClearPad(GPIOA, GPIOA_LED_STM);
}

/*
 * Set intensity
 */
void led_intensity(const uint8_t intensity) {
  if (intensity > 250)
    pwmEnableChannel(&PWMD1, 0, 250);
  else if (intensity == 0)
    pwmDisableChannel(&PWMD1, 0);
  else
    pwmEnableChannel(&PWMD1, 0, intensity);
}

/*
 * Config for the PWM driver
 */
static const PWMConfig ledpwmcfg = {
  100000, /* 100kHz PWM clock frequency.  */
  255,    /* PWM period is 256 ticks.     */
  led_off,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, led_on},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  0,
0
};

/*
 * Blinker thread, times are in milliseconds.
 */
__attribute__((noreturn))
static msg_t blinkerThread(void *arg) {
  (void)arg;

  chRegSetThreadName("blinkerThread");
  while (TRUE) {
    chThdSleepMilliseconds(500);
    led_intensity(250);
    chThdSleepMilliseconds(500);
    led_intensity(0);
  }
}

/*
 * Activate and start driver 1
 */
void led_init() {
  pwmObjectInit(&PWMD1);
  pwmStart(&PWMD1, &ledpwmcfg);

  /*
   * Create the Blinker Thread
   */
  static WORKING_AREA(waBlinkerThread, 128);
  chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread), NORMALPRIO-10, blinkerThread, NULL);
}
