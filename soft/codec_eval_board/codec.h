#ifndef CODEC_H
#define CODEC_H

#include "ch.h"
#include "hal.h"

extern Semaphore sem_codec;
extern int32_t *codec_buffer;
void codec_init(void);

#endif
