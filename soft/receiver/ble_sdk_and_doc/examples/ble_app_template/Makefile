

# Use shell to find name of root folder. Possible but horrible to do in make.
PROJECT_NAME := $(shell basename "$(realpath .)")

DEVICE = NRF51
BOARD = BOARD_PCA10001
TARGET_CHIP := NRF51822_QFAA_G0
#SEGGER_SERIAL =

USE_SOFTDEVICE = s110
DEVICESERIES = xxaa
NRF_DEBUG = TRUE

SDK_REAL_PATH := ../../ble_sdk_5.2.0/
SDK_PATH = $(SDK_REAL_PATH)nrf51822/
TEMPLATE_PATH = $(SDK_REAL_PATH)../pure-gcc-setup/template/

CFLAGS = -g3 -Wall -Werror -Wno-unused-local-typedefs
CFLAGS += -O3 -Ofast
CFLAGS += -DBLE_STACK_SUPPORT_REQD

ifeq ($(NRF_DEBUG), TRUE)
CFLAGS += -DDEBUG_NRF_USER
endif

ASMFLAGS =  -g3

GDB_PORT_NUMBER = 2331
TOOLCHAIN_PATH = 

# List all source files the application uses.
BLE_SRCS =				\
	ble_advdata.c			\
	ble_conn_params.c		\
	ble_srv_common.c		\
	ble_error_log.c 		\
	softdevice_handler.c	    	\

ifeq ($(NRF_DEBUG), TRUE)
BLE_SRCS += ble_debug_assert_handler.c
endif


SDK_APPLICATION_SRCS = 			\
	app_button.c			\
	app_timer.c			\
	app_gpiote.c			\
	app_scheduler.c  		\
	pstorage.c			\
	crc16.c 			\

PROJECT_SRCS =				\
	main.c				\


APPLICATION_SRCS += $(BLE_SRCS) 	\
		$(PROJECT_SRCS)		\
		$(SDK_APPLICATION_SRCS)	\

include $(TEMPLATE_PATH)Makefile


