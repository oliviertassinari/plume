#include "plume_receiver.h"
#include "plume_api.h"
#include "debug_utils.h"
#include "calculator_interface_nodejs.h"
#include <math.h>

extern "C"{
    #include "dof.h"
}

// : needs a pointer to the emmiter and the receiver ble id
PlumeReceiver::PlumeReceiver(PlumeApi *const _plumeApi, const char _MAC[6]){
    plumeApi = _plumeApi;

    // Initialize the mutexes
    pthread_mutex_init(&voltageMutex, NULL);
    pthread_mutex_init(&DOFMutex, NULL);

    octant = 0;
    positionLast = gsl_matrix_alloc(3, 1);
    gsl_matrix_set_zero(positionLast);
    signeRef[0] = 1;
    signeRef[1] = 1;
    signeRef[2] = 1;
    signeRef[3] = 1;
    signeRef[4] = 1;
    signeRef[5] = 1;
    signeRef[6] = 1;
    signeRef[7] = 1;
    signeRef[8] = 1;
    
    orientationLast = gsl_matrix_alloc(3, 1);
    gsl_matrix_set_zero(orientationLast);

    position = gsl_matrix_alloc(3, 1);
    gsl_matrix_set_zero(position);

    orientation = gsl_matrix_alloc(3, 1);
    gsl_matrix_set_zero(orientation);

    // Set receiver calibration, identity because calibration is taken into account on the plume
    calibration = gsl_matrix_alloc(3,3);
    gsl_matrix_set_identity(calibration);

    voltages = gsl_matrix_alloc(3, 3);
    gsl_matrix_set_zero(voltages);

    for (int i = 0; i < 6; ++i){
        MAC[i] = _MAC[i];
    }

    battery = 0; // Unknown
    state = STATE_UNKNOWN; // Unknown
    enable = ENABLE_UNKNOWN; // Unknown
}

PlumeReceiver::~PlumeReceiver(){
    gsl_matrix_free(calibration);
    gsl_matrix_free(position);
    gsl_matrix_free(orientation);
    gsl_matrix_free(positionLast);
    gsl_matrix_free(orientationLast);
    gsl_matrix_free(voltages);

    pthread_mutex_destroy(&voltageMutex);
    pthread_mutex_destroy(&DOFMutex);
}

void PlumeReceiver::connect(void){
    while(state != STATE_CONNECT){
        setState(STATE_CONNECT);
        usleep(1000000); // 1s
    }
}

void PlumeReceiver::start(void){
    connect();
    setEnable(ENABLE_STANDBY);

    double __calibration[6];
    plumeApi->getCalibrationDB()->getCalibrationReceiver(__calibration, MAC);

    uint8_t _calibration[18];

    for(int i = 0; i < 6; ++i){
        _calibration[3*i+2] = ((int)(__calibration[i]*pow(2,19)));
        _calibration[3*i+1] = ((int)(__calibration[i]*pow(2,19))) >> 8;
        _calibration[3*i] = ((int)(__calibration[i]*pow(2,19))) >> 16;
    }

    (void) _calibration;

    interfaceNodejsSetFrequency(MAC, plumeApi->getFrequency());
    interfaceNodejsSetCalibration(MAC, _calibration);

    usleep(1000000); // 1s

    setEnable(ENABLE_ON);
}

void PlumeReceiver::setState(const uint8_t _state){
    interfaceNodejsSetState(MAC, _state);
}

void PlumeReceiver::setEnable(const uint8_t enable){
    interfaceNodejsSetEnable(MAC, enable);
}

void PlumeReceiver::treatVoltages(const double _voltages[9]){
    pthread_mutex_lock(&voltageMutex);
    pthread_mutex_lock(&DOFMutex);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            gsl_matrix_set(voltages, i, j, _voltages[i+3*j]);
        }
    }

    gsl_matrix *emitterCalibration = gsl_matrix_alloc(3, 3);
    PlumeEmitter *emitter = plumeApi->getEmitterCurrent();

    if(emitter){
        gsl_matrix *voltagesFiltred = gsl_matrix_alloc(3, 3);
        emitter->getCalibration(emitterCalibration);

        // Maybe we will need to reverse calibration and emitterCalibration
        dof(position, orientation, voltages, calibration, emitterCalibration, &octant, positionLast, orientationLast, signeRef);

        gsl_matrix_free(voltagesFiltred);
    }

    gsl_matrix_free(emitterCalibration);

    pthread_mutex_unlock(&DOFMutex);
    pthread_mutex_unlock(&voltageMutex);
}

// Return the DOF
void PlumeReceiver::get6DOFRad(double position_dst[3], double orientation_dst[3]){
    pthread_mutex_lock(&DOFMutex);

    for(int i = 0; i < 3; i++){
        position_dst[i] = gsl_matrix_get(position, i, 0);
        orientation_dst[i] = gsl_matrix_get(orientation, i, 0)*M_PI/180.0;
    }

    pthread_mutex_unlock(&DOFMutex);
}

void PlumeReceiver::get6DOFDeg(double position_dst[3], double orientation_dst[3]){
    pthread_mutex_lock(&DOFMutex);

    for(int i = 0; i < 3; i++){
        position_dst[i] = gsl_matrix_get(position, i, 0);
        orientation_dst[i] = gsl_matrix_get(orientation, i, 0);
    }

    pthread_mutex_unlock(&DOFMutex);
}

void PlumeReceiver::getVoltages(double voltages_dst[9]){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            voltages_dst[i+3*j] = gsl_matrix_get(voltages, i, j);
        }
    }
}

// Extern C for wrapper into other languages
extern "C" {
#include <string.h>

    void PlumeReceiverStart(PlumeApi* const plumeApi, const char* const MAC){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        plumeReceiver->start();
    }

    void PlumeReceiverGet6DOFDeg(PlumeApi* const plumeApi, const char* const MAC, double position[3], double orientation[3]){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        plumeReceiver->get6DOFDeg(position, orientation);
    }

    void PlumeReceiverGet6DOFRad(PlumeApi* const plumeApi, const char* const MAC, double position[3], double orientation[3]){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        plumeReceiver->get6DOFRad(position, orientation);
    }

    int PlumerReceiverGetBattery(PlumeApi* const plumeApi, const char* const MAC){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        return plumeReceiver->battery;
    }

    int PlumerReceiverGetState(PlumeApi* const plumeApi, const char* const MAC){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        return plumeReceiver->state;
    }

    void PlumeReceiverSetState(PlumeApi* const plumeApi, const char* const MAC, const uint8_t state){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        plumeReceiver->setState(state);
    }

    void PlumerReceiverGetVoltages(PlumeApi* const plumeApi, const char* const MAC, double voltages[9]){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        plumeReceiver->getVoltages(voltages);
    }

    int PlumerReceiverGetButton(PlumeApi* const plumeApi, const char* const MAC){
        PlumeReceiver* plumeReceiver = plumeApi->getPlumeReceiver(MAC);
        return plumeReceiver->button;
    }
}