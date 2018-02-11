#include "ch.h"
#include "hal.h"

#include "hbridge.h"

#define FREQUENCY_OSCILLATION 18000

// PWM for HB_IN1 et HB_IN2
static const PWMConfig pwmcfg = {
    1800000, // Timer clock in Hz
    (int)(1800000/((float)FREQUENCY_OSCILLATION)), // PWM period in ticks
    NULL,
    {
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW, NULL },
        { PWM_OUTPUT_DISABLED, NULL },
        { PWM_OUTPUT_DISABLED, NULL }
    },
    0,
    0,
    0
};

// State machine for HB_EN
static int state;

void hbridgeCallback(GPTDriver *gptp) {
    (void)gptp;

    if(state == 0){
        state++;
        palSetPad(GPIOB, GPIOB_HB_EN1);
    }
    else if(state == 1){
        state++;
        palSetPad(GPIOB, GPIOB_HB_EN2);
        palClearPad(GPIOB, GPIOB_HB_EN1);
    }
    else if(state == 2){
        state++;
        palSetPad(GPIOB, GPIOB_HB_EN3);
        palClearPad(GPIOB, GPIOB_HB_EN2);
    }
    else if(state == 3){
        state = 0;
        palClearPad(GPIOB, GPIOB_HB_EN3);
    }
}

static const GPTConfig gptcfg = {
    80000,    // Timer clock in Hz
    hbridgeCallback,   // Timer callback
    0
};

// Determine if the hbridge is on or off
static int start;
static int frequency = 200; // Default frequency of position (Hz)

void hbridgeStart(void) {
    if(!start){
        start = 1;
        pwmStart(&PWMD1, &pwmcfg);
        pwmEnableChannel(&PWMD1, 0, (int)(1800000/(2*(float)FREQUENCY_OSCILLATION)));
        pwmEnableChannel(&PWMD1, 1, (int)(1800000/(2*(float)FREQUENCY_OSCILLATION)));

        gptStart(&GPTD4, &gptcfg);
        gptStartContinuous(&GPTD4, (int)(80000/(4*(float)frequency)));
    }
}

void hbridgeSetFrequency(uint8_t _frequency){
    frequency = _frequency;

    if(start == 1){
        gptChangeInterval(&GPTD4, (int)(80000/(4*(float)frequency)));
    }
}

void hbridgeStop(void) {
    if(start){
        gptStopTimer(&GPTD4);
        palClearPad(GPIOB, GPIOB_HB_EN1);
        palClearPad(GPIOB, GPIOB_HB_EN2);
        palClearPad(GPIOB, GPIOB_HB_EN3);
        gptStop(&GPTD4);

        pwmStop(&PWMD1);
        start = 0;
    }
}