#ifndef _DEBUG_H
#define _DEBUG_H

#include "ch.h"
#include "chprintf.h"
extern BaseSequentialStream *chp_debug;

#ifdef DEBUG
#define debug(...) chprintf(chp_debug, __VA_ARGS__)
#else
#define debug(...)
#endif

#ifdef DEBUG
#define DEBUG_ENTERING() chprintf(chp_debug,">>>>>> Entering %s\r\n",__func__)
#else
#define DEBUG_ENTERING()
#endif

#ifdef DEBUG
#define DEBUG_LEAVING() chprintf(chp_debug,"<<<<<< Leaving %s\r\n",__func__)
#else
#define DEBUG_LEAVING()
#endif

#endif
