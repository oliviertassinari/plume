#include "ch.h"
#include "hal.h"
#include "debug.h"
/* Driver for the chprintf debug. */
BaseSequentialStream *chp_debug = (BaseSequentialStream *)&SD2;
