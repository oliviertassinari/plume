#include "communication.h"
#include "ch.h"
#include "hal.h"
#include "debug.h"
#include "communication_type.h"
#include "nrf.h"

static WORKING_AREA(wa_communication_event_mb, 128);
static MAILBOX_DECL(communication_event_mb, wa_communication_event_mb,
        sizeof(wa_communication_event_mb)/sizeof(msg_t));
/* Global variables */
// emitting frequency (if 0, then set to default)
int new_freq = 0;
// new calibration data
float new_calib[6];

/* listening thread */

static WORKING_AREA(waComListeningThread,256);
static msg_t com_listening_thread(void* arg){
    (void) arg;
    unsigned int size = 0;
    static uint8_t msg[128];
    while(1){
        size = nrf_get_frame(msg);
        if(size){
            switch(msg[0]){
                case ON_MSG:
                    com_put_event(CMD_ON);
                    break;
                case OFF_MSG:
                    com_put_event(CMD_OFF);
                    break;
                case STANDBY_MSG:
                    com_put_event(CMD_STANDBY);
                    break;
                case FREQ_MSG:
                    if(size != 2)
                        break;
                    new_freq = msg[1];
                    com_put_event(CMD_FREQ);
                    break;
                case CALIB_MSG:
                    if(size != 19) {
			debug("Com : Calib, Wrong size\r\n");
                        break;
		    }
                    for (int index = 0; index < 6; index++){
                        //32 is the max value, it can be modified later, if we need bigger calibration values
                        new_calib[index] = ((msg[3*index+1]<<16)+(msg[3*index+2]<<8)+msg[3*index+3])*((float)32/(1<<24));
                    }
                    com_put_event(CMD_CALIB);
                    break;
            }
        }
    }
    return 0;
}

void com_init(void){
    nrf_init();
    chThdCreateStatic(waComListeningThread, sizeof(waComListeningThread), NORMALPRIO, com_listening_thread, NULL);
}

/* frame format :
 *         4bits         1bit
 *           v             v
 *[type][bit_shift|xxx|sign2,2][sign2,1|...|sign0,0][value0,0MSB][value0,0LSB][...][value2,2MSB][value2,2LSB]
 */
void com_send_measures(const uint32_t* const measure, const uint16_t signs, const uint8_t flags){
    uint8_t frame[21];
    unsigned int multiplier;
    unsigned int measure_max_msb;
    frame[0] = MEASURE_MSG;
    measure_max_msb = 0;
    frame[1] = signs>>8;
    frame[2] = signs;
    //we detect the higher significant bit amongst all measures
    for(unsigned int meas_index = 0; meas_index<9;meas_index++)
        measure_max_msb |= measure[meas_index];
    for(multiplier = 0; multiplier<16;multiplier++)
        if(!((0xFFFF0000<<multiplier) & measure_max_msb))
            break;
    frame[1] |= multiplier<<4;
    frame[1] |= (flags<<1) & 0xE;
    for(unsigned int meas_index = 0; meas_index<9;meas_index++){
        frame[2*meas_index+3] = measure[meas_index]>>(multiplier+8);
        frame[2*meas_index+4] = (measure[meas_index]>>multiplier)&0xFF;
    }
    nrf_send_frame(frame,21);
}

msg_t com_wait_event(void) {
    msg_t ret;
    chMBFetch(&communication_event_mb, &ret, TIME_INFINITE);
    return ret;
}

void com_put_event(const msg_t evt) {
    chMBPost(&communication_event_mb, evt, 1000);
}

void com_put_event_I(const msg_t evt) {
    chMBPostI(&communication_event_mb, evt);
}
