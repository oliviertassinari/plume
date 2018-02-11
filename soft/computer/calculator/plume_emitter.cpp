#include "plume_api.h"
#include "plume_emitter.h"

extern "C" {
    #include "emitter_interface.h"
    #include "debug_utils.h"
}

PlumeEmitter::PlumeEmitter(PlumeApi* const _plumeApi, const char* const _MAC, const char *const portname){
    DEBUG_ENTERING();

    // Initialize the mutexes
    pthread_mutex_init(&writeMutex, NULL);

    plumeApi = _plumeApi;
    MAC = _MAC;
    fileDescriptor = emitterInterfaceStart(portname);

    double _calibration[3];
    plumeApi->getCalibrationDB()->getCalibrationEmitter(_calibration, _MAC);

    calibration = gsl_matrix_alloc(3,3);
    gsl_matrix_set_identity(calibration);
    for(int i = 0; i < 3; ++i){
        gsl_matrix_set(calibration, i, i, _calibration[i]);
    }

    DEBUG_LEAVING();
}

PlumeEmitter::~PlumeEmitter(){
    DEBUG_ENTERING();
    gsl_matrix_free(calibration);
    pthread_mutex_destroy(&writeMutex);
    DEBUG_LEAVING();
}

void PlumeEmitter::startLed(void){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceStartLed(fileDescriptor);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::stopLed(void){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceLedStop(fileDescriptor);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::setLed(uint8_t octant[2][2][2][3]){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceSetLed(fileDescriptor, octant);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::setLedBrightless(const uint8_t BC){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceSetLedBrightless(fileDescriptor, BC);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::startHbridge(void){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceSetHbridgeFrequency(fileDescriptor, plumeApi->getFrequency());
    emitterInterfaceStartHbridge(fileDescriptor);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::stopHbridge(void){
    pthread_mutex_lock(&writeMutex);
    emitterInterfaceStopHbridge(fileDescriptor);
    pthread_mutex_unlock(&writeMutex);
}

void PlumeEmitter::getCalibration(gsl_matrix * const dst) const{
    gsl_matrix_memcpy(dst, calibration);
    DEBUG_PRINT_MATRIX(("Calibration:\n", calibration));
}

// Extern C for wrapper into other languages
extern "C" {
    void PlumeEmitterStartLed(PlumeEmitter* const plumeEmitter){
        plumeEmitter->startLed();
    }

    void PlumeEmitterStopLed(PlumeEmitter* const plumeEmitter){
        plumeEmitter->stopLed();
    }

    void PlumeEmitterSetLed(PlumeEmitter* const plumeEmitter, uint8_t octant[2][2][2][3]){
        plumeEmitter->setLed(octant);
    }

    void PlumeEmitterSetLedBrightless(PlumeEmitter* const plumeEmitter, uint8_t BC){
        plumeEmitter->setLedBrightless(BC);
    }
}
