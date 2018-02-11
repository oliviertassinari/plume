#ifndef CALCULATOR_INTERFACE_NODEJS_H
#define CALCULATOR_INTERFACE_NODEJS_H

#include <functional>

using namespace std;

/**
 * Start the sockets and the thread that handle callbacks.
 * @param _callbackVoltage (function)
 *                         Callback when we receive voltage.
 * @param _callbackBattery (function)
 *                         Callback when we receive battery.
 * @param _callbackState (function)
 *                       Callback when we receive state.
 * @param _callbackButton (function)
 *                       Callback when we receive new button state.
 */
void interfaceNodejsStart(function<void(char*, double[9])> _callbackVoltage, function<void(char*, uint8_t)> _callbackBattery, function<void(char*, uint8_t)> _callbackState , function<void(char*, uint8_t)> _callbackButton);

/**
 * Set the desired state for the ble.
 * @param MAC (char[6])
 *            Address MAC of the receiver.
 * @param state (uint8_t)
 *              State that we want to set for the receiver.
 */
void interfaceNodejsSetState(char MAC[6], uint8_t state);

/**
 * set the desired enable state of the receiver.
 * @param MAC (char*)
 *            Address MAC of the receiver.
 * @param enable (uint8_t)
 *              State that we want to set for the receiver.
 */
void interfaceNodejsSetEnable(char MAC[6], uint8_t enable);

/**
 * Start to scan devices.
 */
void interfaceNodejsScanDevice(void);

/**
 * Set the frequency of position's computation.
 * @param MAC (char*)
 *            Address MAC of the receiver.
 * @param frequency (uint8_t)
 *                  Frequency of position.
 *                  (Default 200 Hz)
 */
void interfaceNodejsSetFrequency(char MAC[6], uint8_t frequency);

/**
 * Set the calibration of position's computation.
 * @param MAC (char*)
 *            Address MAC of the receiver.
 * @param calibration (uint8_t[3*6])
 *                    We have 6 calibration values for the receiver and 3 bytes per value.
 */
void interfaceNodejsSetCalibration(char MAC[6], uint8_t calibration[18]);

/**
 * Stop the thread.
 */
void interfaceNodejsStop(void);

/**
 * Close the sockets.
 */
void interfaceNodejsClose(void);

#endif