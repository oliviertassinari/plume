#ifndef USB_SERIAL_SHELL_H
#define USB_SERIAL_SHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "shell.h"
#include "chprintf.h"

#include "custom_commands.h"

void usb_serial_init(void);

#endif
