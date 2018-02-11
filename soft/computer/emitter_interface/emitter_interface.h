#ifndef EMITTER_INTERFACE_H
#define EMITTER_INTERFACE_H

/**
 * Start the usb port.
 * @param  portname (char*)
 *                  Name of the port.
 * @return (int)
 *         Return the file descriptor.
 */
int emitterInterfaceStart(const char * const portname);

/**
 * Start the control of the led on the emitter.
 * @param fileDescriptor (int)
 *                       File descriptor.
 */
void emitterInterfaceStartLed(int fileDescriptor);

/**
 * Stop the control of the led on the emitter.
 * @param fileDescriptor (int)
 *                       File descriptor.
 */
void emitterInterfaceLedStop(int fileDescriptor);

/**
 * Send the value of the 8 led to the emitter.
 * @param fileDescriptor (int)
 *                       File descriptor.
 * @param octant (uint8_t[2][2][2][3])
 *               Value of the 8 led following this partern : x, y, z, RGB.
 */
void emitterInterfaceSetLed(const int fileDescriptor, uint8_t octant[2][2][2][3]);

/**
 * Send the global brightless for the 8 led.
 * @param fileDescriptor (int)
 *                       File descriptor.
 * @param BC (uint8_t)
 *           This is the global brightless.
 */
void emitterInterfaceSetLedBrightless(int fileDescriptor, uint8_t BC);

/**
 * Start the control of the hbridge, and the generation of the magnetic field.
 * @param fileDescriptor (int)
 *                       File descriptor.
 */
void emitterInterfaceStartHbridge(int fileDescriptor);

/**
 * Set the frequency of position's computation.
 * @param fileDescriptor (int)
 *           File descriptor.
 * @param frequency (uint8_t)
 *                  Frequency of position.
 *                  (Default 200 Hz)
 */
void emitterInterfaceSetHbridgeFrequency(int fileDescriptor, uint8_t frequency);

/**
 * Stop the hbridge and the generation of the magnetic field.
 * @param fileDescriptor (int)
 *                       File descriptor.
 */
void emitterInterfaceStopHbridge(int fileDescriptor);

#endif
