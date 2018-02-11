#ifndef PLUME_API_H
#define PLUME_API_H

#include <iostream>
#include <string>
#include <cstring>
#include <stdint.h>
#include <vector>
#include <map>
#include <functional>
#include <unistd.h>

#include "plume_emitter.h"
#include "plume_receiver.h"
#include "calibration_db.h"

#define STATE_UNKNOWN 0
#define STATE_CONNECT 1
#define STATE_DISCONNECT 2
#define STATE_TRY 3

#define TYPE_DOF 0
#define TYPE_BATTERY 1
#define TYPE_STATE 2
#define TYPE_BUTTON 3

using namespace std;

class PlumeApi{
    public:
        PlumeApi(const char* const jsonFile);
        ~PlumeApi();

        /**
         * Set a function that will be call as soon as a new data from the PlumeReceivers is receive.
         * @param callback (int, PlumeReceivers*)
         *                 Function that will be called.
         *                 The first parameter is the type of the event.
         *                 TYPE_DOF : Position and orientation
         *                 TYPE_BATTERY : Battery Level
         *                 TYPE_STATE : State
         *                 TYPE BUTTON : Button pressed
         *                 The second is a pointer to the PlumeReceiver object.
         */
        void setCallback(function<void(int, PlumeReceiver*)> callback);

        /**
         * Add an PlumeEmitter to the system.
         * @param MAC (char *)
         *             MAC address.
         * @param portname (char *)
         *                 Is the name of the usb drive were the emitter is connected.
         */
        PlumeEmitter* addEmitter(const char *const MAC, const char *const portname);

        /**
         * Get the emitter that correspond to the last volatages received.
         * @return  (PlumeEmitter *)
         */
        PlumeEmitter* getEmitterCurrent(void);

        /**
         * Start emitters.
         */
        void startEmitters(void) const;

        /**
         * Stop emitters.
         */
        void stopEmitters(void) const;

        /**
         * Start known receivers.
         */
        void startReceivers(void) const;

        /**
         * Try to start a receiver with his MAC address.
         * He will stop trying after 5 fails.
         * @param  MAC (char [6])
         *             MAC address.
         * @return     Return 1 if he succeed, 0 if he failed.
         */
        int startReceiver(const char MAC[6]) const;

        /**
         * Stop known receivers.
         */
        void stopReceivers(void)  const;

        /**
         * Clean everything up.
         */
        void end(void) const;

        /**
         * Start to scan devices.
         */
        void scanDevice(void) const;

        /**
         * Set the frequency of position's computation.
         * Should be set before the start of emitters and receivers.
         * @param frequency (uint8_t)
         *                  Frequency of position.
         *                  (Default 200 Hz)
         */
        void setFrequency(const uint8_t _frequency);

        /**
         * Return the frequency.
         * @return  (uint8_t)
         *          Frequency
         */
        uint8_t getFrequency(void) const;

        /**
         * Return a pointer to the class that store calibration datas.
         * @return  Pointer to the class.
         */
        CalibrationDB* getCalibrationDB(void) const;

        /**
         * Return the plume receiver from a MAC address.
         * @param  MAC (char [6])
         *             MAC address.
         * @return     Pointer to the plume receiver.
         */
        PlumeReceiver* getPlumeReceiver(const char MAC[6]) const;

        vector<PlumeEmitter*> plumeEmitters;
        vector<PlumeReceiver*> plumeReceivers;

    private:
        static void callbackDefault(int type, PlumeReceiver* plumeReceiver);

        void callbackReceiverVoltage(const char[6], const double[9]);
        void callbackReceiverBattery(const char[6], const uint8_t);
        void callbackReceiverState(const char[6], const uint8_t);
        void callbackReceiverButton(const char[6], const uint8_t);
        PlumeReceiver* getPlumeReceiverOrCreate(const char MAC[6]);
        function<void(int, PlumeReceiver*)> callback;

        CalibrationDB *calibrationDB;
        uint8_t frequency;
};

#endif