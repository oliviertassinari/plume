#ifndef MAGNETIC_H
#define MAGNETIC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include "debug_utils.h"

/*Computess the theoric normalized magnetic field given the position*/
void theoric_normalized_field(
                gsl_matrix *const dst, 
                const gsl_matrix *const pos);

#endif
