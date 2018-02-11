#include "magnetic.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include "debug_utils.h"

void theoric_normalized_field(gsl_matrix * const dst, const gsl_matrix * const pos){
    DEBUG_ENTERING();
    double p[3] = {pos->data[0], pos->data[pos->tda], pos->data[2*pos->tda]};
    double r = sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
    double u = pow(r,-5.0);

    int i,j;

    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            gsl_matrix_set(dst,i,j,u*(3*p[i]*p[j]-(i==j)*r*r));

    DEBUG_PRINT_MATRIX(("Normalized theoric field:\n",dst));
    DEBUG_LEAVING();
}
