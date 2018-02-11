#include <iostream>
#include "trajectory_simulation.h"
#include "plume_emitter.h"
#include "plume_receiver.h"

extern "C" {
    #include "dof.h"
    #include "debug_utils.h"
}

#define FREQUENCY 200.0
#define ITERATIONS  1000
#define NOISE       1e-6

using namespace std;

int main(int argc, char **argv){
    (void) argc;
    (void) argv;
    uint8_t currentOctant=0;
    uint8_t nextOctant=0;
    // Show next values values to see if octant has really changed
    uint8_t showNext=0;
    // First calibration matrix 
    double array_c1[]=
    {   1.0,    0.0,    0.0,
                    0.0,    1.2,    0.0,
                    0.0,    0.0,    0.9};
    // Second calibration matrix
    double array_c2[]=
    {   800,    0.0,    0.0,
                    0.0,    1000,   0.0,
                    0.0,    0.0,    1200};

    gsl_matrix_view c1=gsl_matrix_view_array(array_c1,3,3);
    gsl_matrix_view c2=gsl_matrix_view_array(array_c2,3,3);
    gsl_matrix *true_position=gsl_matrix_alloc(3,1);
    gsl_matrix *position=gsl_matrix_alloc(3,1);
    gsl_matrix *orientation=gsl_matrix_alloc(3,1);
    gsl_matrix *voltages=gsl_matrix_alloc(3,3);
    TrajectorySimulation *ts=new TrajectorySimulation(5.0,5.0,0,0,0,0.2,0.0,NOISE,FREQUENCY,&c1.matrix,&c2.matrix);
    PlumeEmitter *pe=new PlumeEmitter("Emitter", array_c2);
    PlumeReceiver *pr=new PlumeReceiver(pe,"Receiver", array_c1);

    for(int i=0;i<ITERATIONS;i++){
        // Orientation is not used at the moment
        ts->next(voltages,true_position,orientation);
        pr->treatVoltages(voltages);
        nextOctant=pr->getNextOctant();
        if(nextOctant!=currentOctant||showNext){
            if(!showNext)
                printf("Change from octant %x to %x\n",currentOctant,nextOctant);
            printf("True position\n");
            matrix_print(true_position);
            pr->get6DOFDeg(position,orientation);
            printf("Raw estimated position\n");
            matrix_print(position);
            pr->getFiltered6DOF(position,orientation);
            printf("Filtered estimated position\n");
            matrix_print(position);
            printf("###########################\n");
            // show next values of necessary or set it to 0
            showNext=1-showNext;
        }
        currentOctant=nextOctant;
    }

    gsl_matrix_free(true_position);
    gsl_matrix_free(position);
    gsl_matrix_free(orientation);
    gsl_matrix_free(voltages);

    delete pr;
    delete pe;
    delete ts;

    return 0;
}