#include "PlumeEmitter.h"

PlumeEmitter::PlumeEmitter(const string _id, const gsl_matrix * const _cal){
        DEBUG_ENTERING();
        cal=gsl_matrix_alloc(3,3);
        gsl_matrix_memcpy(cal,_cal);
        id=_id;
        DEBUG_LEAVING();
}

PlumeEmitter::~PlumeEmitter(){
        DEBUG_ENTERING();
        gsl_matrix_free(cal);
        DEBUG_LEAVING();
}

void PlumeEmitter::getCalib(gsl_matrix * const dst) const{
        DEBUG_ENTERING();
        gsl_matrix_memcpy(dst,cal);
        DEBUG_LEAVING();
}  
