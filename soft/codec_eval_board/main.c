#include "ch.h" 
#include "hal.h"
#include "codec.h"
#include "usb_serial_shell.h"

int main(void){
    halInit();
	chSysInit();
    usb_serial_init();
	codec_init();
        
		while(1){
        palTogglePad(GPIOD, GPIOD_LED4);
        palTogglePad(GPIOD, GPIOD_LED5);
        palTogglePad(GPIOD, GPIOD_LED6);
        chThdSleepMilliseconds(500);
    }
}
