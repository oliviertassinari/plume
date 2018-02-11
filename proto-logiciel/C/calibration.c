#include "calibration.h"

calibration_measure *calibration_measure_alloc(){
    
    return NULL;
}

void calibration_measure_free(calibation_measure *cm){
    

}


double dof_calibrate(gsl_matrix * const cal1, gsl_matrix * const cal2, calibration_set *s, uint32_t method){
    (void) cal1;
    (void) cal2;
    (void) s;
    (void) method;
    return 0.0;
}
