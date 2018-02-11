#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "plume_api.h"

using namespace std;

void callback(int type, PlumeReceiver* plumeReceiver){
    printf("-----\n\r");

    if(type == TYPE_DOF){
        double position[3];
        double orientation[3];

        plumeReceiver->get6DOFDeg(position, orientation);

        printf("new DOF ");
        cout << plumeReceiver->MAC << endl;
        printf("position %f, %f, %f\n\r", position[0], position[1], position[2]);
        printf("orientation %f, %f, %f\n\r", orientation[0], orientation[1], orientation[2]);
    }
    else if(type == TYPE_BATTERY){
        printf("new battery ");
        cout << plumeReceiver->MAC;
        printf(" %u\n\r", plumeReceiver->battery);
    }
    else if(type == TYPE_STATE){
        printf("new state ");
        cout << plumeReceiver->MAC;
        printf(" %u\n\r", plumeReceiver->state);
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

    cout << "test plume API" << endl;

    plumeApi = new PlumeApi("../../calculator/calibration.json");

    signal(SIGINT, signalHandler);

    PlumeEmitter* plumeEmitter0 = plumeApi->addEmitter("1", "/dev/ttyACM0");

    plumeApi->setFrequency(50);
    plumeApi->setCallback(bind(callback, _1, _2));
    plumeApi->scanDevice();

    // Wait one receiver
    while(plumeApi->plumeReceivers.size() < 1){
        usleep(5000);
    }

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