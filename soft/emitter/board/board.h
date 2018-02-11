/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Plume emitter STM32F103CBT6 board.
 */

/*
 * Board identifier.
 */
#define BOARD_NAME              "Plume emitter STM32F103CBT6"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            0
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F10X_MD

/*
 * IO pins assignments.
 */
#define GPIOA_LD_GSCK               0
#define GPIOA_LD_GSLAT              2
#define GPIOA_SPI1NSS               4
#define GPIOA_LD_GSSCK              5
#define GPIOA_LD_GSSIN              7
#define GPIOA_HB_IN1                8
#define GPIOA_USB_ENABLE            10
#define GPIOA_USB_DM                11
#define GPIOA_USB_DP                12

#define GPIOB_LD_XBLNK              0
#define GPIOB_HB_EN3                1
#define GPIOB_LED                   9
#define GPIOB_HB_EN2                12
#define GPIOB_HB_EN1                13
#define GPIOB_HB_IN2                14

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/**
 * CRL : Configuration Register Low.
 * For GPIO from 0 to 7.
 *
 * CRH : Configuration Register High.
 * For GPIO from 8 to 15.
 *
 * ODR : Output Data Register.
 * When configured as output, the value written to the Output Data register.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0  - Alternate push pull output 50MHz  (TIM2_CH1).
 * PA2  - Push Pull output 2MHz      (GPIO).    
 * PA5  - Alternate push pull output 50MHz (SPI1_SCK).
 * PA7  - Alternate push pull output 50MHz (SPI1_MOSI).
 * 
 * PA8  - Alternate push pull output 2MHz  (TIM1_CH1).
 * PA10 - Push Pull output 2MHz      (GPIO).
 * PA11 - Digital input              (USB DM).
 * PA12 - Digital input              (USB DP).
 */
#define VAL_GPIOACRL            0xB8B8828B      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x8884428A      /* PA15...PA8 */
#define VAL_GPIOAODR            0x00000000

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0  - Push Pull output 2MHz      (GPIO).
 * PB1  - Push Pull output 2MHz      (GPIO).
 * PB9  - Push Pull output 2MHz      (GPIO).
 * PB12 - Push Pull output 2MHz      (GPIO).
 * PB13 - Push Pull output 2MHz      (GPIO).
 * PB14 - Alternate push pull output 2MHz (TIM1_CH2N).
 */
#define VAL_GPIOBCRL            0x88888822      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x8A228828      /* PB15...PB8 */
#define VAL_GPIOBODR            0x00000000

/*
 * Port C setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOCCRL            0x88888888      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88888888      /* PC15...PC8 */
#define VAL_GPIOCODR            0x00000000

/*
 * Port D setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIODCRL            0x88888888      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x88888888      /* PD15...PD8 */
#define VAL_GPIODODR            0x00000000

/*
 * Port E setup.
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x88888888      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x88888888      /* PE15...PE8 */
#define VAL_GPIOEODR            0x00000000


/*
 * USB bus activation macro, required by the USB driver.
 */
#define usb_lld_connect_bus(usbp) palSetPad(GPIOA, GPIOA_USB_ENABLE)

/*
 * USB bus de-activation macro, required by the USB driver.
 */
#define usb_lld_disconnect_bus(usbp) palClearPad(GPIOA, GPIOA_USB_ENABLE)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */