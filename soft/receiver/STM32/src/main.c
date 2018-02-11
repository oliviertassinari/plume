//#include <string.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"
#include "my_uart.h"

#include "leds.h"
#include "codec.h"
#include "measure.h"
#include "communication.h"
#include "serial_shell.h"

/*
 * Control thread definition
 */
static WORKING_AREA(waControlThread, 1024);
__attribute__((noreturn))
static msg_t control_thread(void* arg){
    (void) arg;
    msg_t signaled;

    /* state = 0 : stop
     *         1 : standby
     *         2 : measures running
     */
    int state = 0;

    //Init measure thread
    //Make it sleep (not yet)
    measure_init(50);
    while(1) {
	signaled = com_wait_event();
	switch (signaled) {
	case MEASURES_READY:

	    chMtxLock(&mtx_ext_measures);
        uint8_t button_1 = palReadPad(GPIOD, GPIOD_BUTTON_1) ? 0 : 1;
        uint8_t button_2 = palReadPad(GPIOD, GPIOD_BUTTON_2) ? 0 : 1;
	    com_send_measures(ext_measure, 0x0, ((button_1 << 1) | button_2));
	    chMtxUnlock();
        break;

	case CMD_OFF:
	    if(state){
		if(state == 2)
		    measure_stop();
		state = 0;
	    }
	    break;

	case CMD_ON:
	    if(state != 2){
		// Initialize the measure
		state = measure_start(50) ? 1 : 2;
	    }
	    break;

	case CMD_STANDBY:
	    if(state == 2){
		measure_stop();
	    }
	    state = 1;
	    break;

	case CMD_FREQ:
	    if (state==2){
		set_emit_frequency(50);
	    }
	    break;

	case CMD_CALIB:
	    measure_set_calib(new_calib);
	    break;
	}
    }
}

/*
 * Drivers declarations
 */

int main(void) {
    halInit();
    chSysInit();
    /*
     * Activate LED on board
     */
    led_init();
    /*
     * Initialize communication thread
     */
    com_init();
    /*
     * Activate the control thread
     */
    chThdCreateStatic(waControlThread, sizeof(waControlThread), NORMALPRIO, control_thread, NULL);

    chThdSleepMilliseconds(TIME_INFINITE);
}
