#include "ch.h"
#include "hal.h"

#include "led.h"

#include "chprintf.h"
#include "usb_slave.h"

// Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSB first)
static const SPIConfig hs_spicfg = {
    NULL,
    GPIOA,
    GPIOA_SPI1NSS,
    0
};

// PWM for LD_GSCK
static const PWMConfig pwmcfg = {
    2000000, // Timer clock in Hz
    10, // PWM period in ticks
    NULL,
    {
        { PWM_OUTPUT_ACTIVE_HIGH, NULL },
        { PWM_OUTPUT_DISABLED, NULL },
        { PWM_OUTPUT_DISABLED, NULL },
        { PWM_OUTPUT_DISABLED, NULL }
    },
    0,
    0,
    0
};

void ledStart(void){
    pwmStart(&PWMD2, &pwmcfg);
    pwmEnableChannel(&PWMD2, 0, 5);

    palSetPad(GPIOB, GPIOB_LD_XBLNK);
}

void ledSendLatch1(uint8_t data[]){
    palClearPad(GPIOB, GPIOB_LD_XBLNK); // Turn off OUT and reset counter
    palClearPad(GPIOA, GPIOA_LD_GSLAT); // Select destination register : Grayscale Data Latch

    // SPI TX buffers for GSSIN
    uint8_t latch1buf[36]; // 288 bits

    unsigned i;
    for(i=0; i < 12; i++){
        latch1buf[35-3*i] = data[2*i];
        latch1buf[35-3*i-1] = data[2*i+1] << 4;
        latch1buf[35-3*i-2] = data[2*i+1] >> 4;
    }

    spiStart(&SPID1, &hs_spicfg);
    spiSend(&SPID1, 36, latch1buf);
    spiStop(&SPID1);

    // Select destination register
    palSetPad(GPIOA, GPIOA_LD_GSLAT);
    palClearPad(GPIOA, GPIOA_LD_GSLAT);
    palSetPad(GPIOB, GPIOB_LD_XBLNK); // Turn on OUT
}

void ledSendLatch2(uint8_t BC){
    palClearPad(GPIOB, GPIOB_LD_XBLNK); // Turn off OUT and reset counter
    palSetPad(GPIOA, GPIOA_LD_GSLAT); // Select destination register : DC/BC/FC/UD Data Latch

    uint8_t latch2buf[25]; // 200 bits

    // Dot Correction
    unsigned i;
    for(i=0; i < 21; i++){
        latch2buf[21-i+4] = 0xff; // Full
    }

    // Global Brightness Control
    latch2buf[3] = (float)BC*0.6; // Red
    latch2buf[2] = (float)BC*0.6; // Green
    latch2buf[1] = BC; // Blue

    // Function Control Data Latch
    latch2buf[0] = 0xff; // ALL 1

    spiStart(&SPID1, &hs_spicfg);
    spiSend(&SPID1, 25, latch2buf);
    spiStop(&SPID1);

    // Select destination register
    palClearPad(GPIOA, GPIOA_LD_GSLAT);
    palSetPad(GPIOA, GPIOA_LD_GSLAT);
    palSetPad(GPIOB, GPIOB_LD_XBLNK); // Turn on OUT
}

void ledStop(void){
    palClearPad(GPIOB, GPIOB_LD_XBLNK);

    pwmStop(&PWMD2);
}