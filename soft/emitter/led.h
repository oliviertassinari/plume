#ifndef LED_H
#define LED_H

/**
 * Start the leds.
 */
void ledStart(void);

/**
 * Send data to the first latch : the Grayscale Data Latch.
 * @param data (uint8_t[])
 *             Data to send.
 */
void ledSendLatch1(uint8_t data[]);

/**
 * Send data to the second latch : DC/BC/FC/UD Data Latch.
 * @param BC (uint8_t)
 *           Brigthness of the leds.
 */
void ledSendLatch2(uint8_t BC);

/**
 * Stop the leds.
 */
void ledStop(void);

#endif