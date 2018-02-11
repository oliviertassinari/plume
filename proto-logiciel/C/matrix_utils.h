#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include "debug_utils.h"

void matrix_invert(
                gsl_matrix *const dst, 
                const gsl_matrix *const src);
void matrix_print(const gsl_matrix *const m);

#endif
