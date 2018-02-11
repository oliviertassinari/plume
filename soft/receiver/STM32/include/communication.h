#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "ch.h"

#define CMD_ON 0x2
#define CMD_OFF 0x4
#define CMD_STANDBY 0x8
#define CMD_FREQ 0x10
#define CMD_CALIB 0x20


void com_init(void);
void com_test(void);
void com_send_measures(const uint32_t *const measures, const uint16_t signs, const uint8_t flags);
msg_t com_wait_event(void);
void com_put_event_I(const msg_t evt);
void com_put_event(const msg_t evt);

// new settings values
extern int new_freq;
extern float new_calib[6];

#endif //COMMUNICATION_H
