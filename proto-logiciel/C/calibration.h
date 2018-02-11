#ifndef CALIBRATION
#define CALIBRATION

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl_blas.h>

#define DISTANCE_METHOD 1
#define FIELD_METHOD    2

//Linked structure for the measures
typedef struct calibration_measure{
    double t; //distance along the rail vector
    gsl_matrix *voltages;
    struct calibration_measure *next;
} calibration_measure;

typedef struct calibration_set{
    gsl_matrix *position; //Approximate position of the rail
    gsl_matrix *direction; //Approximate direction of the rail
    calibration_measure *measures;
} calibration_set;

//Allocates a struct and its sub matrices. Sets next to NULL
calibration_measure *calibration_measure_alloc();

//Frees a struct and all its sub matrices
void calibration_measure_free(calibation_measure *cm);

//Computes the calibration matrices given a linked list of measure.
//The given calibration martices must contain approximative calibration values
//Two methods can be chosen:
//Minimize on the sum of squared distances
//minimize on the sum of squared fields
double dof_calibrate(gsl_matrix * const cal1, gsl_matrix * const cal2, calibration_set *s, uint32_t method);

#endif
