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
 * Setup for STMicroelectronics STM32F427
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F427
#define BOARD_NAME                  "Plume_Receiver"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                19200000
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330

/*
 * MCU type as defined in the ST header.
 */
#define STM32F427_437xx

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0
#define GPIOA_PIN1                  1
#define GPIOA_CODEC_CS_1            2
#define GPIOA_LED_STM               3
#define GPIOA_CODEC_WS_3            4
#define GPIOA_CODEC_SCK             5
#define GPIOA_CODEC_MISO            6
#define GPIOA_CODEC_MOSI            7
#define GPIOA_PIN8                  8
#define GPIOA_PIN9                  9
#define GPIOA_PIN10                 10
#define GPIOA_PIN11                 11
#define GPIOA_PIN12                 12
#define GPIOA_SWDIO                 13
#define GPIOA_SWCLK                 14
#define GPIOA_PIN15                 15

#define GPIOB_CODEC_nRST_2          0
#define GPIOB_INT_2                 1
#define GPIOB_BOOT1                 2
#define GPIOB_SWO                   3
#define GPIOB_PIN4                  4
#define GPIOB_MPU_INT               5
#define GPIOB_MPU_SCL               6
#define GPIOB_MPU_SDA               7
#define GPIOB_PIN8                  8
#define GPIOB_PIN9                  9
#define GPIOB_CODEC_CK_2            10
#define GPIOB_PIN11                 11
#define GPIOB_CODEC_WS_2            12
#define GPIOB_PIN13                 13
#define GPIOB_PIN14                 14
#define GPIOB_PIN15                 15

#define GPIOC_PIN0                  0
#define GPIOC_PIN1                  1
#define GPIOC_PIN2                  2
#define GPIOC_CODEC_DOUT_2          3
#define GPIOC_CODEC_CS_2            4
#define GPIOC_PIN5                  5
#define GPIOC_CODEC_MCK_2           6
#define GPIOC_CODEC_MCK_3           7
#define GPIOC_PIN8                  8
#define GPIOC_PIN9                  9
#define GPIOC_CODEC_CK_3            10
#define GPIOC_PIN11                 11
#define GPIOC_CODEC_DOUT_3          12
#define GPIOC_CODEC_INT_1           13
#define GPIOC_CODEC_nRST_1          14
#define GPIOC_PIN15                 15

#define GPIOD_PIN0                  0
#define GPIOD_PIN1                  1
#define GPIOD_PIN2                  2
#define GPIOD_PIN3                  3
#define GPIOD_PIN4                  4
#define GPIOD_BUTTON_1              5
#define GPIOD_BUTTON_2              6
#define GPIOD_PIN7                  7
#define GPIOD_PIN8                  8
#define GPIOD_PIN9                  9
#define GPIOD_PIN10                 10
#define GPIOD_PIN11                 11
#define GPIOD_CODEC_INT_3           12
#define GPIOD_CODEC_nRST_3          13
#define GPIOD_PIN14                 14
#define GPIOD_CODEC_CS_3            15

#define GPIOE_UART_NRF_to_STM       0
#define GPIOE_UART_STM_to_NRF       1
#define GPIOE_CODEC_MCK_1           2
#define GPIOE_PIN3                  3
#define GPIOE_CODEC_WS_1            4
#define GPIOE_CODEC_CK_1            5
#define GPIOE_CODEC_DOUT_1          6
#define GPIOE_PIN7                  7
#define GPIOE_PIN8                  8
#define GPIOE_PIN9                  9
#define GPIOE_PIN10                10
#define GPIOE_PIN11                11
#define GPIOE_PIN12                12
#define GPIOE_PIN13                13
#define GPIOE_PIN14                14
#define GPIOE_PIN15                15

#define GPIOF_PIN0                  0
#define GPIOF_PIN1                  1
#define GPIOF_PIN2                  2
#define GPIOF_PIN3                  3
#define GPIOF_PIN4                  4
#define GPIOF_PIN5                  5
#define GPIOF_PIN6                  6
#define GPIOF_PIN7                  7
#define GPIOF_PIN8                  8
#define GPIOF_PIN9                  9
#define GPIOF_PIN10                 10
#define GPIOF_PIN11                 11
#define GPIOF_PIN12                 12
#define GPIOF_PIN13                 13
#define GPIOF_PIN14                 14
#define GPIOF_PIN15                 15

#define GPIOG_PIN0                  0
#define GPIOG_PIN1                  1
#define GPIOG_PIN2                  2
#define GPIOG_PIN3                  3
#define GPIOG_PIN4                  4
#define GPIOG_PIN5                  5
#define GPIOG_PIN6                  6
#define GPIOG_PIN7                  7
#define GPIOG_PIN8                  8
#define GPIOG_PIN9                  9
#define GPIOG_PIN10                 10
#define GPIOG_PIN11                 11
#define GPIOG_PIN12                 12
#define GPIOG_PIN13                 13
#define GPIOG_PIN14                 14
#define GPIOG_PIN15                 15

#define GPIOH_OSC_IN                0
#define GPIOH_OSC_OUT               1
#define GPIOH_PIN2                  2
#define GPIOH_PIN3                  3
#define GPIOH_PIN4                  4
#define GPIOH_PIN5                  5
#define GPIOH_PIN6                  6
#define GPIOH_PIN7                  7
#define GPIOH_PIN8                  8
#define GPIOH_PIN9                  9
#define GPIOH_PIN10                 10
#define GPIOH_PIN11                 11
#define GPIOH_PIN12                 12
#define GPIOH_PIN13                 13
#define GPIOH_PIN14                 14
#define GPIOH_PIN15                 15

#define GPIOI_PIN0                  0
#define GPIOI_PIN1                  1
#define GPIOI_PIN2                  2
#define GPIOI_PIN3                  3
#define GPIOI_PIN4                  4
#define GPIOI_PIN5                  5
#define GPIOI_PIN6                  6
#define GPIOI_PIN7                  7
#define GPIOI_PIN8                  8
#define GPIOI_PIN9                  9
#define GPIOI_PIN10                 10
#define GPIOI_PIN11                 11
#define GPIOI_PIN12                 12
#define GPIOI_PIN13                 13
#define GPIOI_PIN14                 14
#define GPIOI_PIN15                 15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 * PA0  - PIN0                      (input pullup).
 * PA1  - PIN1                      (input pullup).
 * PA2  - CODEC_CS_1                (output pullup).
 * PA3  - LED_STM                   (output pullup).
 * PA4  - CODEC_WS_3                (alternate 6 : I2S3_WS).
 * PA5  - CODEC_SCK                 (alternate 5 : SPI1_CK).
 * PA6  - CODEC_MISO                (alternate 5 : SPI1_MISO).
 * PA7  - CODEC_MOSI                (alternate 5 : SPI1_MOSI).
 * PA8  - PIN8                      (input pullup).
 * PA9  - PIN9                      (input pullup).
 * PA10 - PIN10                     (input pullup).
 * PA11 - PIN11                     (input pullup).
 * PA12 - PIN12                     (input pullup).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PIN15                     (input pullup).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_OUTPUT(GPIOA_PIN0) |            \
                                     PIN_MODE_INPUT(GPIOA_PIN1) |            \
                                     PIN_MODE_OUTPUT(GPIOA_CODEC_CS_1) |     \
                                     PIN_MODE_OUTPUT(GPIOA_LED_STM) |	     \
                                     PIN_MODE_ALTERNATE(GPIOA_CODEC_WS_3) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_CODEC_SCK) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_CODEC_MISO) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_CODEC_MOSI) |  \
                                     PIN_MODE_INPUT(GPIOA_PIN8) |            \
                                     PIN_MODE_INPUT(GPIOA_PIN9) |            \
                                     PIN_MODE_INPUT(GPIOA_PIN10) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN11) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN12) |           \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |       \
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PIN0) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CODEC_CS_1) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_STM) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CODEC_WS_3) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CODEC_SCK) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CODEC_MISO) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_CODEC_MOSI) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN9) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN10) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN11) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN12) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_25M(GPIOA_PIN0) |            \
                                     PIN_OSPEED_25M(GPIOA_PIN1) |            \
                                     PIN_OSPEED_100M(GPIOA_CODEC_CS_1) |      \
                                     PIN_OSPEED_25M(GPIOA_LED_STM) |         \
                                     PIN_OSPEED_25M(GPIOA_CODEC_WS_3) |      \
                                     PIN_OSPEED_100M(GPIOA_CODEC_SCK) |       \
                                     PIN_OSPEED_100M(GPIOA_CODEC_MISO) |      \
                                     PIN_OSPEED_100M(GPIOA_CODEC_MOSI) |      \
                                     PIN_OSPEED_25M(GPIOA_PIN8) |            \
                                     PIN_OSPEED_25M(GPIOA_PIN9) |            \
                                     PIN_OSPEED_25M(GPIOA_PIN10) |           \
                                     PIN_OSPEED_25M(GPIOA_PIN11) |           \
                                     PIN_OSPEED_25M(GPIOA_PIN12) |           \
                                     PIN_OSPEED_25M(GPIOA_SWDIO) |           \
                                     PIN_OSPEED_25M(GPIOA_SWCLK) |           \
                                     PIN_OSPEED_25M(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(GPIOA_PIN0) |          \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN1) |          \
                                     PIN_PUPDR_PULLUP(GPIOA_CODEC_CS_1) |    \
                                     PIN_PUPDR_PULLUP(GPIOA_LED_STM) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_CODEC_WS_3) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_CODEC_SCK) |   \
                                     PIN_PUPDR_PULLDOWN(GPIOA_CODEC_MISO) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_CODEC_MOSI) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN8) |          \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN9) |          \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN10) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN10) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN11) |         \
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PIN0) |              \
                                     PIN_ODR_HIGH(GPIOA_PIN1) |              \
                                     PIN_ODR_HIGH(GPIOA_CODEC_CS_1) |        \
                                     PIN_ODR_HIGH(GPIOA_LED_STM) |           \
                                     PIN_ODR_HIGH(GPIOA_CODEC_WS_3) |        \
                                     PIN_ODR_HIGH(GPIOA_CODEC_SCK) |         \
                                     PIN_ODR_HIGH(GPIOA_CODEC_MISO) |        \
                                     PIN_ODR_HIGH(GPIOA_CODEC_MOSI) |        \
                                     PIN_ODR_HIGH(GPIOA_PIN8) |              \
                                     PIN_ODR_HIGH(GPIOA_PIN9) |              \
                                     PIN_ODR_HIGH(GPIOA_PIN10) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN11) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN12) |             \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |             \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PIN0, 0) |            \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0) |            \
                                     PIN_AFIO_AF(GPIOA_CODEC_CS_1, 0) |      \
                                     PIN_AFIO_AF(GPIOA_LED_STM, 1) |         \
                                     PIN_AFIO_AF(GPIOA_CODEC_WS_3, 6) |      \
                                     PIN_AFIO_AF(GPIOA_CODEC_SCK, 5) |       \
                                     PIN_AFIO_AF(GPIOA_CODEC_MISO, 5) |      \
                                     PIN_AFIO_AF(GPIOA_CODEC_MOSI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0) |            \
                                     PIN_AFIO_AF(GPIOA_PIN9, 0) |            \
                                     PIN_AFIO_AF(GPIOA_PIN10, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN11, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN12, 0) |           \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |           \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0))

/*
 * GPIOB setup:
 *
 * PB0  - CODEC_nRST_2              (output pullup).
 * PB1  - CODEC_INT_2               (input floating).
 * PB2  - BOOT1                     (input floating).
 * PB3  - SWO                       (alternate 0).
 * PB4  - PIN4                      (input pullup).
 * PB5  - MPU_INT                   (input pullup).
 * PB6  - MPU_SCL                   (input pullup).
 * PB7  - MPU_SDA                   (input pullup).
 * PB8  - PIN8                      (input pullup).
 * PB9  - PIN9                      (input pullup).
 * PB10 - CODEC_CK_2                (alternate 5 : I2S2_CK).
 * PB11 - PIN11                     (input pullup).
 * PB12 - CODEC_WS_2                (alternate 5 : I2S2_WS).
 * PB13 - PIN13                     (input pullup).
 * PB14 - PIN14                     (input pullup).
 * PB15 - PIN15                     (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_CODEC_nRST_2) |    \
                                     PIN_MODE_INPUT(GPIOB_INT_2) |            \
                                     PIN_MODE_INPUT(GPIOB_BOOT1) |            \
                                     PIN_MODE_ALTERNATE(GPIOB_SWO) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN4) |             \
                                     PIN_MODE_INPUT(GPIOB_MPU_INT) |          \
                                     PIN_MODE_INPUT(GPIOB_MPU_SCL) |          \
                                     PIN_MODE_INPUT(GPIOB_MPU_SDA) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN8) |             \
                                     PIN_MODE_INPUT(GPIOB_PIN9) |             \
                                     PIN_MODE_ALTERNATE(GPIOB_CODEC_CK_2) |   \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |            \
                                     PIN_MODE_ALTERNATE(GPIOB_CODEC_WS_2) |   \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN14) |            \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_CODEC_nRST_2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_INT_2) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_BOOT1) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO) |          \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOB_MPU_INT) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_MPU_SCL) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_MPU_SDA) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN9) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOB_CODEC_CK_2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_CODEC_WS_2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_25M(GPIOB_CODEC_nRST_2) |     \
                                     PIN_OSPEED_25M(GPIOB_INT_2) |            \
                                     PIN_OSPEED_25M(GPIOB_BOOT1) |            \
                                     PIN_OSPEED_25M(GPIOB_SWO) |              \
                                     PIN_OSPEED_25M(GPIOB_PIN4) |             \
                                     PIN_OSPEED_25M(GPIOB_MPU_INT) |          \
                                     PIN_OSPEED_25M(GPIOB_MPU_SCL) |          \
                                     PIN_OSPEED_25M(GPIOB_MPU_SDA) |          \
                                     PIN_OSPEED_25M(GPIOB_PIN8) |             \
                                     PIN_OSPEED_25M(GPIOB_PIN9) |             \
                                     PIN_OSPEED_25M(GPIOB_CODEC_CK_2) |       \
                                     PIN_OSPEED_25M(GPIOB_PIN11) |            \
                                     PIN_OSPEED_25M(GPIOB_CODEC_WS_2) |       \
                                     PIN_OSPEED_25M(GPIOB_PIN13) |            \
                                     PIN_OSPEED_25M(GPIOB_PIN14) |            \
                                     PIN_OSPEED_25M(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_CODEC_nRST_2) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_INT_2) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_BOOT1) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_SWO) |          \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN4) |           \
                                     PIN_PUPDR_PULLUP(GPIOB_MPU_INT) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_MPU_SCL) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_MPU_SDA) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN8) |           \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN9) |           \
                                     PIN_PUPDR_PULLUP(GPIOB_CODEC_CK_2) |     \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN11) |          \
                                     PIN_PUPDR_FLOATING(GPIOB_CODEC_WS_2) |   \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN13) |          \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN14) |          \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_CODEC_nRST_2) |       \
                                     PIN_ODR_HIGH(GPIOB_INT_2) |              \
                                     PIN_ODR_HIGH(GPIOB_BOOT1) |              \
                                     PIN_ODR_HIGH(GPIOB_SWO) |                \
                                     PIN_ODR_HIGH(GPIOB_PIN4) |               \
                                     PIN_ODR_HIGH(GPIOB_MPU_INT) |            \
                                     PIN_ODR_HIGH(GPIOB_MPU_SCL) |            \
                                     PIN_ODR_HIGH(GPIOB_MPU_SDA) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN8) |               \
                                     PIN_ODR_HIGH(GPIOB_PIN9) |               \
                                     PIN_ODR_HIGH(GPIOB_CODEC_CK_2) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |              \
                                     PIN_ODR_HIGH(GPIOB_CODEC_WS_2) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN14) |              \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_CODEC_nRST_2, 0) |     \
                                     PIN_AFIO_AF(GPIOB_INT_2, 0) |            \
                                     PIN_AFIO_AF(GPIOB_BOOT1, 0) |            \
                                     PIN_AFIO_AF(GPIOB_SWO, 0) |              \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0) |             \
                                     PIN_AFIO_AF(GPIOB_MPU_INT, 0) |          \
                                     PIN_AFIO_AF(GPIOB_MPU_SCL, 0) |          \
                                     PIN_AFIO_AF(GPIOB_MPU_SDA, 0))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0) |             \
                                     PIN_AFIO_AF(GPIOB_PIN9, 0) |             \
                                     PIN_AFIO_AF(GPIOB_CODEC_CK_2, 5) |       \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0) |            \
                                     PIN_AFIO_AF(GPIOB_CODEC_WS_2, 5) |       \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0) |            \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0) |            \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (input pullup).
 * PC1  - PIN1                      (input pullup).
 * PC2  - PIN2                      (input pullup).
 * PC3  - CODEC_DOUT_2              (alternate 5 : I2S2_SD).
 * PC4  - CODEC_CS_2                (output pullup).
 * PC5  - PIN5                      (input pullup).
 * PC6  - CODEC_MCK_2               (alternate 5 : I2S2_MCK).
 * PC7  - CODEC_MCK_3               (alterate 6 : I2S3_MCK).
 * PC8  - PIN8                      (input pullup).
 * PC9  - PIN9                      (input pullup).
 * PC10 - CODEC_CK_3                (alternate 6 : I2S3_CK).
 * PC11 - PIN11                     (input pullup).
 * PC12 - CODEC_DOUT_3              (alternate 6 : I2S3_SD).
 * PC13 - CODEC_INT_1               (input floating).
 * PC14 - CODEC_nRST_1              (output pullup).
 * PC15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN2) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_CODEC_DOUT_2)|\
                                     PIN_MODE_OUTPUT(GPIOC_CODEC_CS_2) |    \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_CODEC_MCK_2) |\
                                     PIN_MODE_ALTERNATE(GPIOC_CODEC_MCK_3) |\
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN9) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_CODEC_CK_3) | \
                                     PIN_MODE_INPUT(GPIOC_PIN11) |          \
                                     PIN_MODE_ALTERNATE(GPIOC_CODEC_DOUT_3)|\
                                     PIN_MODE_INPUT(GPIOC_CODEC_INT_1) |    \
                                     PIN_MODE_OUTPUT(GPIOC_CODEC_nRST_1) |  \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_DOUT_2)|\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_CS_2) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_MCK_2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_MCK_3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_CK_3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_DOUT_3)|\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_INT_1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CODEC_nRST_1)|\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_25M(GPIOC_PIN0) |           \
                                     PIN_OSPEED_25M(GPIOC_PIN1) |           \
                                     PIN_OSPEED_25M(GPIOC_PIN2) |           \
                                     PIN_OSPEED_25M(GPIOC_CODEC_DOUT_2) |   \
                                     PIN_OSPEED_100M(GPIOC_CODEC_CS_2) |     \
                                     PIN_OSPEED_25M(GPIOC_PIN5) |           \
                                     PIN_OSPEED_25M(GPIOC_CODEC_MCK_2) |    \
                                     PIN_OSPEED_25M(GPIOC_CODEC_MCK_3) |    \
                                     PIN_OSPEED_25M(GPIOC_PIN8) |           \
                                     PIN_OSPEED_25M(GPIOC_PIN9) |           \
                                     PIN_OSPEED_25M(GPIOC_CODEC_CK_3) |     \
                                     PIN_OSPEED_25M(GPIOC_PIN11) |          \
                                     PIN_OSPEED_25M(GPIOC_CODEC_DOUT_3) |   \
                                     PIN_OSPEED_25M(GPIOC_CODEC_INT_1) |    \
                                     PIN_OSPEED_25M(GPIOC_CODEC_nRST_1) |   \
                                     PIN_OSPEED_25M(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_DOUT_2)|\
                                     PIN_PUPDR_PULLUP(GPIOC_CODEC_CS_2) |   \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN5) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_MCK_2) |\
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_MCK_3) |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN9) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_CK_3) | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN11) |        \
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_DOUT_2)|\
                                     PIN_PUPDR_FLOATING(GPIOC_CODEC_INT_1) |\
                                     PIN_PUPDR_PULLUP(GPIOC_CODEC_nRST_1)|  \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOC_CODEC_DOUT_2) |     \
                                     PIN_ODR_HIGH(GPIOC_CODEC_CS_2) |       \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOC_CODEC_MCK_2) |      \
                                     PIN_ODR_HIGH(GPIOC_CODEC_MCK_3) |      \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOC_CODEC_CK_3) |       \
                                     PIN_ODR_HIGH(GPIOC_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOC_CODEC_DOUT_3) |     \
                                     PIN_ODR_HIGH(GPIOC_CODEC_INT_1) |      \
                                     PIN_ODR_HIGH(GPIOC_CODEC_nRST_1) |     \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_CODEC_DOUT_2, 5) |   \
                                     PIN_AFIO_AF(GPIOC_CODEC_CS_2, 0) |     \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOC_CODEC_MCK_2, 5) |    \
                                     PIN_AFIO_AF(GPIOC_CODEC_MCK_3, 6))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOC_CODEC_CK_3, 6) |     \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOC_CODEC_DOUT_3, 6) |   \
                                     PIN_AFIO_AF(GPIOC_CODEC_INT_1, 0) |    \
                                     PIN_AFIO_AF(GPIOC_CODEC_nRST_1, 0) |   \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0))


/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - BUTTON_1                  (input pullup).
 * PD6  - BUTTON_2                  (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - CODEC_INT_3               (input floating).
 * PD13 - CODEC_nRST_3              (output pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - CODEC_CS_3                (output pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOD_BUTTON_1) |   \
                                     PIN_MODE_INPUT(GPIOD_BUTTON_2) |   \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOD_CODEC_INT_3) |    \
                                     PIN_MODE_OUTPUT(GPIOD_CODEC_nRST_3) |  \
                                     PIN_MODE_INPUT(GPIOD_PIN14) |          \
                                     PIN_MODE_OUTPUT(GPIOD_CODEC_CS_3))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_BUTTON_1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_BUTTON_2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_CODEC_INT_3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_CODEC_nRST_3)|\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_CODEC_CS_3))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_25M(GPIOD_PIN0) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN1) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN2) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN3) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN4) |          \
                                     PIN_OSPEED_25M(GPIOD_BUTTON_1) |      \
                                     PIN_OSPEED_25M(GPIOD_BUTTON_2) |      \
                                     PIN_OSPEED_25M(GPIOD_PIN7) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN8) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN9) |          \
                                     PIN_OSPEED_25M(GPIOD_PIN10) |         \
                                     PIN_OSPEED_25M(GPIOD_PIN11) |         \
                                     PIN_OSPEED_25M(GPIOD_CODEC_INT_3) |   \
                                     PIN_OSPEED_25M(GPIOD_CODEC_nRST_3) |  \
                                     PIN_OSPEED_25M(GPIOD_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOD_CODEC_CS_3))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_BUTTON_1) |   \
                                     PIN_PUPDR_PULLUP(GPIOD_BUTTON_2) |   \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN11) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_CODEC_INT_3) |\
                                     PIN_PUPDR_PULLUP(GPIOD_CODEC_nRST_3)|  \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_CODEC_CS_3))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOD_BUTTON_1) |         \
                                     PIN_ODR_HIGH(GPIOD_BUTTON_2) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOD_CODEC_INT_3) |      \
                                     PIN_ODR_HIGH(GPIOD_CODEC_nRST_3) |     \
                                     PIN_ODR_HIGH(GPIOD_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOD_CODEC_CS_3))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOD_BUTTON_1, 0) |       \
                                     PIN_AFIO_AF(GPIOD_BUTTON_2, 0) |       \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOD_CODEC_INT_3, 0) |    \
                                     PIN_AFIO_AF(GPIOD_CODEC_nRST_3, 0) |   \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOD_CODEC_CS_3, 0))

/*
 * GPIOE setup:
 *
 * PE0  - UART_NRF_to_STM           (alternate 8 : UART8_Rx).
 * PE1  - UART_STM_to_NRF           (alternate 8 : UART8_Tx).
 * PE2  - CODEC_MCK_1               (alternate 6 : SAI1_MCLK_A).
 * PE3  - PIN3                      (input pullup).
 * PE4  - CODEC_WS_1                (alternate 6 : SAI1_FS_A).
 * PE5  - CODEC_CK_1                (alternate 6 : SAI1_SCK_A).
 * PE6  - CODEC_DOUT_1              (alternate 6 : SAI1_SD_A).
 * PE7  - PIN7                      (input pullup).
 * PE8  - PIN8                      (input pullup).
 * PE9  - PIN9                      (input pullup).
 * PE10 - PIN10                     (input pullup).
 * PE11 - PIN11                     (input pullup).
 * PE12 - PIN12                     (input pullup).
 * PE13 - PIN13                     (input pullup).
 * PE14 - PIN14                     (input pullup).
 * PE15 - PIN15                     (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ALTERNATE(GPIOE_UART_NRF_to_STM) |\
                                     PIN_MODE_ALTERNATE(GPIOE_UART_STM_to_NRF)| \
                                     PIN_MODE_ALTERNATE(GPIOE_CODEC_MCK_1) |    \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_CODEC_WS_1) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_CODEC_CK_1) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_CODEC_DOUT_1) |   \
                                     PIN_MODE_INPUT(GPIOE_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_UART_NRF_to_STM)|\
                                     PIN_OTYPE_PUSHPULL(GPIOE_UART_STM_to_NRF)|\
                                     PIN_OTYPE_PUSHPULL(GPIOE_CODEC_MCK_1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_CODEC_WS_1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_CODEC_CK_1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_CODEC_DOUT_1)|\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_25M(GPIOE_UART_NRF_to_STM)|\
                                     PIN_OSPEED_25M(GPIOE_UART_STM_to_NRF)|\
                                     PIN_OSPEED_25M(GPIOE_CODEC_MCK_1) |   \
                                     PIN_OSPEED_25M(GPIOE_PIN3) |          \
                                     PIN_OSPEED_25M(GPIOE_CODEC_WS_1) |    \
                                     PIN_OSPEED_25M(GPIOE_CODEC_CK_1) |    \
                                     PIN_OSPEED_25M(GPIOE_CODEC_DOUT_1) |  \
                                     PIN_OSPEED_25M(GPIOE_PIN7) |          \
                                     PIN_OSPEED_25M(GPIOE_PIN8) |          \
                                     PIN_OSPEED_25M(GPIOE_PIN9) |          \
                                     PIN_OSPEED_25M(GPIOE_PIN10) |         \
                                     PIN_OSPEED_25M(GPIOE_PIN11) |         \
                                     PIN_OSPEED_25M(GPIOE_PIN12) |         \
                                     PIN_OSPEED_25M(GPIOE_PIN13) |         \
                                     PIN_OSPEED_25M(GPIOE_PIN14) |         \
                                     PIN_OSPEED_25M(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_FLOATING(GPIOE_UART_NRF_to_STM)|\
                                     PIN_PUPDR_FLOATING(GPIOE_UART_STM_to_NRF)|\
                                     PIN_PUPDR_FLOATING(GPIOE_CODEC_MCK_1) |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_CODEC_WS_1) | \
                                     PIN_PUPDR_FLOATING(GPIOE_CODEC_CK_1) | \
                                     PIN_PUPDR_FLOATING(GPIOE_CODEC_DOUT_1)|\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN7) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN8) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN9) |       \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN11) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN13) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN14) |      \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_UART_NRF_to_STM) |  \
                                     PIN_ODR_HIGH(GPIOE_UART_STM_to_NRF) |  \
                                     PIN_ODR_HIGH(GPIOE_CODEC_MCK_1) |      \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOE_CODEC_WS_1) |       \
                                     PIN_ODR_HIGH(GPIOE_CODEC_CK_1) |       \
                                     PIN_ODR_HIGH(GPIOE_CODEC_DOUT_1) |     \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_UART_NRF_to_STM, 8) |\
                                     PIN_AFIO_AF(GPIOE_UART_STM_to_NRF, 8) |\
                                     PIN_AFIO_AF(GPIOE_CODEC_MCK_1, 6) |    \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOE_CODEC_WS_1, 6) |     \
                                     PIN_AFIO_AF(GPIOE_CODEC_CK_1, 6) |     \
                                     PIN_AFIO_AF(GPIOE_CODEC_DOUT_1, 6) |   \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input floating).
 * PF1  - PIN1                      (input floating).
 * PF2  - PIN2                      (input floating).
 * PF3  - PIN3                      (input floating).
 * PF4  - PIN4                      (input floating).
 * PF5  - PIN5                      (input floating).
 * PF6  - PIN6                      (input floating).
 * PF7  - PIN7                      (input floating).
 * PF8  - PIN8                      (input floating).
 * PF9  - PIN9                      (input floating).
 * PF10 - PIN10                     (input floating).
 * PF11 - PIN11                     (input floating).
 * PF12 - PIN12                     (input floating).
 * PF13 - PIN13                     (input floating).
 * PF14 - PIN14                     (input floating).
 * PF15 - PIN15                     (input floating).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_100M(GPIOF_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOF_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_100M(GPIOG_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOG_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_100M(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_100M(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_100M(GPIOH_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOH_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input floating).
 * PI1  - PIN1                      (input floating).
 * PI2  - PIN2                      (input floating).
 * PI3  - PIN3                      (input floating).
 * PI4  - PIN4                      (input floating).
 * PI5  - PIN5                      (input floating).
 * PI6  - PIN6                      (input floating).
 * PI7  - PIN7                      (input floating).
 * PI8  - PIN8                      (input floating).
 * PI9  - PIN9                      (input floating).
 * PI10 - PIN10                     (input floating).
 * PI11 - PIN11                     (input floating).
 * PI12 - PIN12                     (input floating).
 * PI13 - PIN13                     (input floating).
 * PI14 - PIN14                     (input floating).
 * PI15 - PIN15                     (input floating).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_100M(GPIOI_PIN0) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN1) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN2) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN3) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN4) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN5) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN6) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN7) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN8) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN9) |          \
                                     PIN_OSPEED_100M(GPIOI_PIN10) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN11) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN12) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN13) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN14) |         \
                                     PIN_OSPEED_100M(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))


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
