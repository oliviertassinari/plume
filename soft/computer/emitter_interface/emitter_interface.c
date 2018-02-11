#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "emitter_interface.h"

static int setInterfaceAttribs(int fileDescriptor, int speed, int parity){
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if(tcgetattr(fileDescriptor, &tty) != 0){
        printf("error %d from tcgetattr\r\n", errno);
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // ignore break signal
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 1;

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS; // no flow control

    // Flush anything already in the serial buffer
    tcflush(fileDescriptor, TCIFLUSH);

    if(tcsetattr(fileDescriptor, TCSANOW, &tty) != 0){
        printf("error %d from tcsetattr\r\n", errno);
        return -1;
    }

    return fileDescriptor;
}

static void sendData(int fileDescriptor, char* data, int len){
    if(write(fileDescriptor, data, len) < 0){
        //printf("error %d write : %s\r\n", errno, strerror(errno));
    }
    else{
        char chr = 'l';
        while(chr != '>'){
            if(read(fileDescriptor, &chr, 1) < 0){
                printf("error %d read : %s\r\n", errno, strerror(errno));
            }
        }
    }
}

int emitterInterfaceStart(char const *const portname){
    int fileDescriptor = open(portname, O_RDWR | O_NOCTTY);

    if(fileDescriptor < 0){
        printf("error %d opening %s: %s\r\n", errno, portname, strerror(errno));
        return fileDescriptor;
    }

    // Set speed to 115,200 bps, 8n1 (no parity)
    fileDescriptor = setInterfaceAttribs(fileDescriptor, B115200, 0);  

    return fileDescriptor;
}

void emitterInterfaceStartLed(int fileDescriptor){
    sendData(fileDescriptor, "led 1\r", 6);
}

void emitterInterfaceLedStop(int fileDescriptor){
    sendData(fileDescriptor, "led 0\r", 6);
}

void emitterInterfaceSetLed(const int fileDescriptor, uint8_t octant[2][2][2][3]){
    char data[6+24] = "led 2\r";

    unsigned x, y, z;
    for(z=0; z < 2; z++){
        for(y=0; y < 2; y++){
            for(x=0; x < 2; x++){
                data[6+3*(x+2*(y+2*z))] = octant[x][y][z][0]; // Red
                data[6+3*(x+2*(y+2*z))+1] = octant[x][y][z][1]; // Green
                data[6+3*(x+2*(y+2*z))+2] = octant[x][y][z][2]; // Blue
            }
        }
    }

    sendData(fileDescriptor, data, 6+24);
}

void emitterInterfaceSetLedBrightless(int fileDescriptor, uint8_t BC){
    char data[7] = "led 3\r";
    data[6] = BC;

    sendData(fileDescriptor, data, 7);
}

void emitterInterfaceStartHbridge(int fileDescriptor){
    sendData(fileDescriptor, "hbridge 1\r", 10);
}

void emitterInterfaceSetHbridgeFrequency(int fileDescriptor, uint8_t frequency){
    char data[11] = "hbridge 2\r";
    data[10] = frequency;

    sendData(fileDescriptor, data, 11);
}

void emitterInterfaceStopHbridge(int fileDescriptor){
    sendData(fileDescriptor, "hbridge 0\r", 10);
}
