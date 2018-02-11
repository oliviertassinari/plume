#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

void cmd_spiread(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_spiwrite(BaseSequentialStream *chp, int argc, char *argv[]);

#endif
