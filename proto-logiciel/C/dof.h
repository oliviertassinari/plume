#ifndef DOF_H
#define DOF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include "matrix_utils.h"
#include "magnetic.h"
#include "debug_utils.h"

double dof(
                gsl_matrix *const position, 
                gsl_matrix *const orientation, 
                const gsl_matrix *const measures, 
                const gsl_matrix *const cal1, 
                const gsl_matrix *const cal2, 
                const uint8_t octant);

void voltages_from_dof(
                gsl_matrix *const voltages,
                const gsl_matrix *const position,
                const gsl_matrix *const orientation,
                const gsl_matrix *const cal1,
                const gsl_matrix *const cal2);

#endif
