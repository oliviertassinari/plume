#ifndef POLYNOMIAL_REGRESSION_H
#define POLYNOMIAL_REGRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include "matrix_utils.h"

/*Computes a polynomial regression at the degree given by the number of column in dst*/
void polynomial_regression(
                gsl_matrix *const ans, 
                const gsl_matrix *const x, 
                const gsl_matrix *const y);

#endif
