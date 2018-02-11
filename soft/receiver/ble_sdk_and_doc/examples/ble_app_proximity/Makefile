

# Use shell to find name of root folder. Possible but horrible to do in make.
PROJECT_NAME := $(shell basename "$(realpath .)")

DEVICE = NRF51
BOARD = BOARD_PCA10001
TARGET_CHIP := NRF51822_QFAA_G0
#SEGGER_SERIAL =

USE_SOFTDEVICE = s110
DEVICESERIES = xxaa

SDK_REAL_PATH := ../../ble_sdk_5.2.0/
SDK_PATH = $(SDK_REAL_PATH)nrf51822/
TEMPLATE_PATH = $(SDK_REAL_PATH)../pure-gcc-setup/template/

CFLAGS = -g3 -Wall -Werror -Wno-unused-local-typedefs
CFLAGS += -O3 -Ofast
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DDEBUG_NRF_USER

ASMFLAGS =  -g3

GDB_PORT_NUMBER = 2331
TOOLCHAIN_PATH = 

# List all source files the application uses.
BLE_SRCS =				\
	ble_conn_params.c		\
	ble_advdata.c			\
	ble_bondmngr.c			\
	ble_ias.c			\
	ble_ias_c.c			\
	ble_lls.c			\
	ble_bas.c			\
	ble_tps.c			\
	ble_srv_common.c		\
	softdevice_handler.c		\

SDK_APPLICATION_SRCS = 			\
	app_button.c			\
	app_timer.c			\
	app_gpiote.c			\
	pstorage.c			\
	crc16.c 			\

PROJECT_SRCS =				\
	main.c				\


APPLICATION_SRCS += $(BLE_SRCS) 	\
		$(PROJECT_SRCS)		\
		$(SDK_APPLICATION_SRCS)	\

include $(TEMPLATE_PATH)Makefile


