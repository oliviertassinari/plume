#include "plume_api.h"

#include "calculator_interface_nodejs.h"

PlumeApi::PlumeApi(const char* const jsonFile){
    using namespace std::placeholders;

    frequency = 200;

    setCallback(bind(callbackDefault, _1, _2));
    calibrationDB = new CalibrationDB(jsonFile);
    interfaceNodejsStart(bind(&PlumeApi::callbackReceiverVoltage, this, _1, _2),
                        bind(&PlumeApi::callbackReceiverBattery, this, _1, _2),
                        bind(&PlumeApi::callbackReceiverState, this, _1, _2),
                        bind(&PlumeApi::callbackReceiverButton, this, _1, _2));
}

PlumeApi::~PlumeApi(){
    end();

    unsigned i;

    for(i = 0; i < plumeEmitters.size(); i++){
        delete plumeEmitters[i];
    }

    for(i = 0; i < plumeReceivers.size(); i++){
        delete plumeReceivers[i];
    }

    plumeEmitters.clear();
    plumeReceivers.clear();

    delete calibrationDB;

    printf("delete PlumeApi\n");
}

void PlumeApi::setCallback(function<void(int, PlumeReceiver*)> _callback){
    callback = _callback;
}

PlumeEmitter* PlumeApi::addEmitter(const char *const MAC, const char *const portname){
    PlumeEmitter* plumeEmitter = new PlumeEmitter(this, MAC, portname);
    plumeEmitters.push_back(plumeEmitter);

    return plumeEmitter;
}

PlumeEmitter* PlumeApi::getEmitterCurrent(void){
    if(plumeEmitters.size() > 0){
        return plumeEmitters[0];
    }
    else{
        return NULL;
    }
}

void PlumeApi::callbackDefault(int type, PlumeReceiver* plumeReceiver){
    (void) type;
    (void) plumeReceiver;
}

void PlumeApi::startEmitters(void) const{
    unsigned i,j;

    for(i = 0, j = plumeEmitters.size(); i < j; i++){
        plumeEmitters[i]->startHbridge();
    }
}

void PlumeApi::stopEmitters(void) const{
    unsigned i,j;

    for(i = 0, j = plumeEmitters.size(); i < j; i++){
        plumeEmitters[i]->stopHbridge();
    }
}

void PlumeApi::startReceivers(void) const{
    unsigned i,j;

    for(i = 0, j = plumeReceivers.size(); i < j; i++){
        plumeReceivers[i]->start();
    }
}

int PlumeApi::startReceiver(const char MAC[6]) const{
    int succed = 0; // Failed
    int numberTry = 0;

    while(numberTry < 10){
        PlumeReceiver* plumeReceiver = getPlumeReceiver(MAC);
        if(plumeReceiver != NULL){
            if(plumeReceiver->state == STATE_CONNECT){
                plumeReceiver->start();
                succed = 1;
                break;
            }
            else{
                plumeReceiver->setState(STATE_CONNECT);
            }
        }
        else{ // Receiver unknown, we need to scan
            scanDevice();
        }
        numberTry++;

        usleep(5000000); // 2s
    }

    return succed;
}

void PlumeApi::stopReceivers(void) const{
    unsigned i,j;

    for(i = 0, j = plumeReceivers.size(); i < j; i++){
        plumeReceivers[i]->setEnable(ENABLE_STANDBY); // Standby
    }
}

void PlumeApi::end(void) const{
    interfaceNodejsStop();
    stopReceivers();
    stopEmitters();
    interfaceNodejsClose();
}

void PlumeApi::scanDevice(void) const{
    interfaceNodejsScanDevice();
}

void PlumeApi::setFrequency(uint8_t _frequency){
    frequency = _frequency;
}

uint8_t PlumeApi::getFrequency(void) const{
    return frequency;
}

CalibrationDB* PlumeApi::getCalibrationDB(void) const{
    return calibrationDB;
}

void PlumeApi::callbackReceiverVoltage(const char MAC[6], const double voltages[9]){
    PlumeReceiver* plumeReceiver = getPlumeReceiverOrCreate(MAC);

    plumeReceiver->treatVoltages(voltages);

    callback(TYPE_DOF, plumeReceiver);
}

void PlumeApi::callbackReceiverBattery(const char MAC[6], const uint8_t battery){
    PlumeReceiver* plumeReceiver = getPlumeReceiverOrCreate(MAC);

    plumeReceiver->battery = battery;

    callback(TYPE_BATTERY, plumeReceiver);
}

void PlumeApi::callbackReceiverState(const char MAC[6], const uint8_t state){
    PlumeReceiver* plumeReceiver = getPlumeReceiverOrCreate(MAC);

    plumeReceiver->state = state;

    callback(TYPE_STATE, plumeReceiver);
}

void PlumeApi::callbackReceiverButton(const char MAC[6], const uint8_t button){
    PlumeReceiver* plumeReceiver = getPlumeReceiverOrCreate(MAC);

    int button1 = 0;
    int button2 = 0;

    // Posedge
    if((button & 0x1) == 0x1 && (plumeReceiver->button & 0x1) == 0){
        button1 = 1;
    }
    if(((button & 0x2) >> 1) == 0x1 && ((plumeReceiver->button & 0x2) >> 1) == 0){
        button2 = 1;
    }

    plumeReceiver->button = button;

    if(button1 == 1 || button2 == 1){
        callback(TYPE_BUTTON, plumeReceiver);
    }
}

PlumeReceiver* PlumeApi::getPlumeReceiver(const char MAC[6])const{
    PlumeReceiver* plumeReceiver = NULL;

    unsigned i,j;
    for(i = 0, j = plumeReceivers.size(); i < j; i++){
        int equal = 1;

        for(int k = 0; k < 6; ++k){
            if(MAC[k] != plumeReceivers[i]->MAC[k]){
                equal = 0;
                break;
            }
        }

        if(equal){
            return plumeReceivers[i];
        }
    }

    return plumeReceiver;
}

PlumeReceiver* PlumeApi::getPlumeReceiverOrCreate(const char MAC[6]){
    PlumeReceiver* plumeReceiver;
    int found = 0;

    unsigned i,j;
    for(i = 0, j = plumeReceivers.size(); i < j; i++){
        int equal = 1;

        for(int k = 0; k < 6; ++k){
            if(MAC[k] != plumeReceivers[i]->MAC[k]){
                equal = 0;
                break;
            }
        }

        if(equal){
            plumeReceiver = plumeReceivers[i];
            found = 1;
            break;
        }
    }

    // No found
    if(!found){
        plumeReceiver = new PlumeReceiver(this, MAC);
        plumeReceivers.push_back(plumeReceiver);
    }

    return plumeReceiver;
}

// Extern C for wrapper into other languages
extern "C" {
    PlumeApi* PlumeApiNew(const char* const jsonFile){
        return new PlumeApi(jsonFile);
    }

    typedef void (*callback_t)(int, char*);

    static callback_t callback;

    void PlumeApiCallbackDefault(const int state, PlumeReceiver* const plumeReceiver){
        callback(state, plumeReceiver->MAC);
    }

    void PlumeApiSetCallback(PlumeApi* plumeApi, callback_t _callback){
        using namespace std::placeholders;
        callback = _callback;
        plumeApi->setCallback(bind(PlumeApiCallbackDefault, _1, _2));
    }

    PlumeEmitter* PlumeApiAddEmitter(PlumeApi* const plumeApi, const char *const MAC, const char *const portname){
        return plumeApi->addEmitter(MAC, portname);
    }

    void PlumeApiStartEmitters(PlumeApi *const  plumeApi){
        plumeApi->startEmitters();
    }

    void PlumeApiStopEmitters(PlumeApi *const plumeApi){
        plumeApi->stopEmitters();
    }

    void PlumeApiStartReceivers(PlumeApi *const plumeApi){
        plumeApi->startReceivers();
    }

    int PlumeApiStartReceiver(PlumeApi *const plumeApi, const char MAC[6]){
        return plumeApi->startReceiver(MAC);
    }

    void PlumeApiStopReceivers(PlumeApi *const plumeApi){
       plumeApi->stopReceivers();
    }

    void PlumeApiEnd(PlumeApi *const plumeApi){
       plumeApi->end();
    }

    void PlumeApiDelete(PlumeApi *const plumeApi){
        delete plumeApi;
    }

    void PlumeApiScanDevice(PlumeApi *const plumeApi){
       plumeApi->scanDevice();
    }

    void PlumeApiSetFrequency(PlumeApi* const plumeApi, const uint8_t frequency){
        plumeApi->setFrequency(frequency);
    }

    int PlumeReceiversSize(PlumeApi* const plumeApi){
        return plumeApi->plumeReceivers.size();
    }

    char* PlumeReceiversAt(PlumeApi* const plumeApi, const int i){
        return plumeApi->plumeReceivers.at(i)->MAC;
    }
}
