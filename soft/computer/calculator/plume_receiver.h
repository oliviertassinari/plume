#ifndef PLUME_RECEIVER_H
#define PLUME_RECEIVER_H

#include <stdint.h>
#include <gsl/gsl_blas.h>

#define ENABLE_UNKNOWN 0
#define ENABLE_ON 0x0E
#define ENABLE_OFF 0x0F
#define ENABLE_STANDBY 0x10

using namespace std;
class PlumeApi;
class PlumeReceiver{
    public: 
        PlumeReceiver(PlumeApi *const _plumeApi, const char _MAC[6]);
        ~PlumeReceiver();

        /**
         * Return the last computed DOF with orientation in degree.
         * @param position_dst (double[9])
         *                     Position array to fill.
         * @param orientation_dst (double[9])
         *                        Orientation array to fill.
         */
        void get6DOFDeg(double position_dst[3], double orientation_dst[3]);

        /**
         * Return the last computed DOF with orientation in radian.
         * @param position_dst (double[9])
         *                     Position array to fill.
         * @param orientation_dst (double[9])
         *                        Orientation array to fill.
         */
        void get6DOFRad(double position_dst[3], double orientation_dst[3]);

        /**
         * Return the last received voltages.
         * @param voltages_dst (double[9])
         *                     Voltages array to fill.
         *                        0                     ....             8
         *                     | ExRx ExRy ExRz EyRx EyRy EyRz EzRx EzRy EzRz |
         */
        void getVoltages(double voltages_dst[9]);

        /**
         * Treat volatages once receive. He will compute the DOF.
         * @param _voltages (double[9])
         *                      0                     ....             8
         *                  | ExRx ExRy ExRz EyRx EyRy EyRz EzRx EzRy EzRz |
         */
        void treatVoltages(const double _voltages[9]);

        /**
         * Connect via BLE to the receiver.
         */
        void connect(void);

        /**
         * Start the receiver to compute volatages.
         */
        void start(void);

        /**
         * Set the desired state for the BLE.
         * @param MAC (char*)
         *            Address MAC of the receiver.
         * @param state (uint8_t)
         *              State that we want to set for the receiver.
         *              List of the possible states for the receiver.
         *              STATE_UNKNOWN : Unknown (can't be set)
         *              STATE_CONNECT : Connect
         *              STATE_DISCONNECT : Disconnect
         *              STATE_TRY : Try to connect (can't be set)
         */
        void setState(const uint8_t _state);

        /**
         * Set the desired enable state of the receiver.
         * @param MAC (char*)
         *            Address MAC of the receiver.
         * @param enable (uint8_t)
         *              State that we want to set for the receiver.
         *              List of the possible states for the receiver.
         *              ENABLE_UNKNOWN : Unknown (can't be set)
         *              ENABLE_ON : On
         *              ENABLE_OFF : Off
         *              ENABLE_STANDBY : Standby
         */
        void setEnable(const uint8_t enable);

        char MAC[6];
        uint8_t battery;
        uint8_t state;
        uint8_t enable;
        uint8_t button;

    private:
        PlumeApi *plumeApi;

        /**
         * Octant in witch the receiver is :
         *          bit 0 : axe X, 0 = +X, 1 = -X
         *          bit 1 : axe Y, 0 = +Y, 1 = -Y
         *          bit 2 : axe Z, 0 = +Z, 1 = -Z
         */
        uint8_t octant;
        gsl_matrix *positionLast;
        gsl_matrix *orientationLast;
        int signeRef[9];

        // DOF
        gsl_matrix *position;
        gsl_matrix *orientation;
        gsl_matrix *calibration;
        gsl_matrix *voltages;

        // Mutexes
        pthread_mutex_t voltageMutex;
        pthread_mutex_t DOFMutex;
};

#endif
