#ifndef PLUMEEMITTER_H
#define PLUMEEMITTER_H

#include <iostream>    
#include <stdlib.h>
#include <string>
#include <gsl/gsl_matrix.h>
extern   "C" {
#include "debug_utils.h"
}

using namespace std;
class PlumeEmitter{
        public:
                /*needs calibration matrix*/
                PlumeEmitter(
                                const string _id, 
                                const gsl_matrix *const _cal);
                ~PlumeEmitter();
                void getCalib(gsl_matrix *const dst) const ;
        private:
                gsl_matrix *cal;
                string id;
};

#endif
