#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "calculator_interface_nodejs.h"

using namespace std;

void callbackReceiverVoltage(char* MAC, double voltage[9]){
    printf("voltage %s\r\n", MAC);
    printf("%g, %g, %g\r\n", voltage[0], voltage[1], voltage[2]);
    printf("%g, %g, %g\r\n", voltage[3], voltage[4], voltage[5]);
    printf("%g, %g, %g\r\n", voltage[6], voltage[7], voltage[8]);
}

void callbackReceiverBattery(char* MAC, uint8_t battery){
    printf("battery %s %u\r\n", MAC, battery);
}

void callbackReceiverState(char* MAC, uint8_t state){
    printf("state %s %u\r\n", MAC, state);
}

void signalHandler(int sig){
    (void) sig;

    printf("You pressed Ctrl+C - or killed me with -2\n");

    interfaceNodejsStop();

    exit(0);
}

int main(int argc, const char* argv[]){
    (void) argc;
    (void) argv;

    using namespace std::placeholders;

    cout << "test ok" << endl;

    interfaceNodejsStart(bind(callbackReceiverVoltage, _1, _2),
                                            bind(callbackReceiverBattery, _1, _2),
                                            bind(callbackReceiverState, _1, _2));

    signal(SIGINT, signalHandler);

    while(1){
        interfaceNodejsSetState("MACMAC", 50);
        usleep(50000);
    }

    return 0;
}