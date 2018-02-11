#ifndef HBRIDGE_H
#define HBRIDGE_H

/**
 * Start the generation of the magnetic field.
 */
void hbridgeStart(void);

/**
 * Change the frequency of position.
 * @param _frequency (uint8_t)
 *                   Frequency of position in Hertz.
 */
void hbridgeSetFrequency(uint8_t _frequency);

/**
 * Stop the generation of the magnetic field.
 */
void hbridgeStop(void);

#endif