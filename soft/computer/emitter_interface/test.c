#include <stdio.h>
#include <stdint.h>

#include "emitter_interface.h"

int main(int argc, const char* argv[]){
    (void) argc;
    (void) argv;

    int fd = emitterInterfaceStart("/dev/ttyACM0");
    //emitterInterfaceStartHbridge(fd);
    emitterInterfaceSetHbridgeFrequency(fd, 200);
    emitterInterfaceStartLed(fd);
    emitterInterfaceSetLedBrightless(fd, 100);

    uint8_t octant[2][2][2][3];

    unsigned x, y, z;
    for(z=0; z < 2; z++){
        for(y=0; y < 2; y++){
            for(x=0; x < 2; x++){
                octant[x][y][z][0] = 50;
                octant[x][y][z][1] = 50;
                octant[x][y][z][2] = 0;
            }
        }
    }

    octant[0][0][0][0] = 50;
    octant[1][0][0][0] = 50;

    octant[0][1][0][1] = 50;
    octant[1][1][0][1] = 50;

    octant[0][0][0][1] = 50;
    octant[1][0][0][2] = 50;
    octant[0][1][0][0] = 50;
    octant[1][1][0][2] = 50;


    emitterInterfaceSetLed(fd, octant);

    printf("test ok\r\n");

    return 0;
}