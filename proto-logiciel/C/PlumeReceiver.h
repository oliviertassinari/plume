#ifndef PLUMERECEIVER_H
#define PLUMERECEIVER_H
#include <iostream>    
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <gsl/gsl_matrix.h>
#include "PlumeEmitter.h"
#include "CircularBuffer.h"
extern   "C"{
#include "dof.h"
#include "polynomial_regression.h"
}

#define FREQUENCY       200.0

using namespace std;
class PlumeReceiver{
        public: 
                /*Constructor: needs a pointer to the emmiter and the receiver ble id*/
                PlumeReceiver(
                                PlumeEmitter *_emmiter, 
                                const string _id, 
                                const gsl_matrix *const _cal);
                ~PlumeReceiver();
                /*returns the filtered dof*/
                void getFiltered6DOF(
                                gsl_matrix *const position_dst, 
                                gsl_matrix *const orientation_dst);
                /*returns the filtered dof*/
                void getRaw6DOF(
                                gsl_matrix *const position_dst, 
                                gsl_matrix *const orientation_dst);
                /*Return the 3x3 voltage matrix*/
                void treatVoltages(
                                const gsl_matrix *const _voltages);
                /*Return next Octant*/
                uint8_t getNextOctant() const;
        private:
                /*Compute the dof*/
                void compute6DOF(const gsl_matrix *const voltages);
                /*Estimates position of the receiver given time, 0.0 is now*/
                void estimatePosition(
                                gsl_matrix *const position_dst, 
                                gsl_matrix * const orientation_dst, 
                                const double secs);
                /*Computes the polynomial coefficients*/
                void compute6DOFPolynoms();
                /*Computes the quadratic-filtered DOF filtered* */
                void computeFiltered6DOF();
                /*Computes what the next position octant will be*/
                void computeNextOctant();
                /*Is the position up-to-date in regard of the voltages?*/
                bool up_to_date;
                PlumeEmitter *emitter;
                string id;
                uint8_t nextOctant;
                /*Receiver calibration matrix*/
                gsl_matrix *cal;
                gsl_matrix *position;
                gsl_matrix *orientation;
                /*Filtered with polynom*/
                gsl_matrix *filteredPosition;
                gsl_matrix *filteredOrientation;
                //gsl_matrix *tmp;
                /*Estimation polynoms*/
                gsl_matrix *positionPolynom;
                gsl_matrix *orientationPolynom;
                /*Mutexes*/
                pthread_mutex_t voltageMutex;
                pthread_mutex_t DOFMutex;
                /*Circular Buffers*/
                CircularBuffer *positionBuffer;
                CircularBuffer *orientationBuffer;
};
#endif
