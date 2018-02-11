#ifndef PLUME_EMITTER_H
#define PLUME_EMITTER_H

#include <gsl/gsl_matrix.h>

using namespace std;

class PlumeApi;
class PlumeEmitter{
    public:
        PlumeEmitter(PlumeApi* const _plumeApi, const char* const _MAC, const char *const portname);
        ~PlumeEmitter();

        void getCalibration(gsl_matrix *const dst) const;

        /**
        * Start the control of the led on the emitter.
        */
        void startLed(void);

        /**
        * Stop the control of the led on the emitter.
        */
        void stopLed(void);

        /**
        * Send the value of the 8 led to the emitter.
        * @param octant (uint8_t[2][2][2][3])
        *               Value of the 8 led following this partern : i, j, k, RGB.
        */
        void setLed(uint8_t octant[2][2][2][3]);

        /**
        * Send the global brightless for the 8 led.
        * @param BC (uint8_t)
        *           This is the global brightless.
        */
        void setLedBrightless(const uint8_t BC);

        /**
        * Start the control of the hbridge, and the generation of the magnetic field.
        */
        void startHbridge(void);

        /**
        * Stop the hbridge and the generation of the magnetic field.
        */
        void stopHbridge(void);

    private:
        PlumeApi* plumeApi;
        const char* MAC;
        int fileDescriptor;
        gsl_matrix *calibration;
        pthread_mutex_t writeMutex;
};

#endif
