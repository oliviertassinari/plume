#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "plume_api.h"

using namespace std;

void callback(int type, PlumeReceiver* plumeReceiver){
    (void) plumeReceiver;
    if(type == 2){ // State
        struct timeval stop;
        gettimeofday(&stop, NULL);
        uint32_t value1 = (uint32_t)stop.tv_sec;
        uint32_t value2 = (uint32_t)stop.tv_usec;

        printf("%u %u\n\r", value1, value2);
    }
}

static PlumeApi* plumeApi;

void signalHandler(int sig){
    (void) sig;

    printf("You pressed Ctrl+C - or killed me with -2\n");

    delete plumeApi;

    exit(0);
}

int main(int argc, char **argv){
    (void) argc;
    (void) argv;

    using namespace std::placeholders;

    cout << "test ok" << endl;

    double calibrationEmitter[9] = { 800,    0.0,    0.0,
                                                                    0.0,    1000,   0.0,
                                                                    0.0,    0.0,    1200 };

    plumeApi = new PlumeApi();

    signal(SIGINT, signalHandler);

    PlumeEmitter* plumeEmitter0 = plumeApi->addEmitter("/dev/ttyACM0", calibrationEmitter);
    plumeEmitter0->startLed();

    plumeApi->setFrequency(100);
    plumeApi->setCallback(bind(callback, _1, _2));
    plumeApi->scanDevice();

    // Wait one receiver
    while(plumeApi->plumeReceivers.size() < 1);

    plumeApi->startEmitters();
    plumeApi->plumeReceivers[0]->start();

    while(1){
        unsigned i,j;
        for(i = 0, j = plumeApi->plumeReceivers.size(); i < j; i++){
            cout << plumeApi->plumeReceivers[i]->MAC << endl;
        }
        usleep(50000);
    }

    return 0;
}