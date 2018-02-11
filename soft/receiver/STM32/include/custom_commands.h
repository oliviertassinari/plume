#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

#include "ch.h"
#include "hal.h"

void cmd_spiread(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_spiwrite(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_set_amp(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_get_amp(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_reset_codecs(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_codecread(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_codecwrite(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_startmeasure(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_stopmeasure(BaseSequentialStream *chp, int argc, char *argv[]);

#endif
