#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#include "dof.h"
#include "matrix_utils.h"

double dof(gsl_matrix * const position, gsl_matrix * const orientation, gsl_matrix * const measures, const gsl_matrix *const cal1, const gsl_matrix * const cal2, uint8_t *octant, gsl_matrix * const positionLast, gsl_matrix * const orientationLast, int signeRef[9]){

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(signeRef[3*i+j] == -1){
                gsl_matrix_set(measures, i, j, -1*gsl_matrix_get(measures, i, j));
            }
        }
    }

    // Compute normalized field
    gsl_matrix *cal1Invers = gsl_matrix_alloc(3, 3); 
    gsl_matrix_set_zero(cal1Invers);
    for (int i = 0; i < 3; ++i){
        gsl_matrix_set(cal1Invers, i, i, 1.0/gsl_matrix_get(cal1, i, i));
    }

    gsl_matrix *cal2Invers = gsl_matrix_alloc(3, 3); 
    gsl_matrix_set_zero(cal2Invers);
    for (int i = 0; i < 3; ++i){
        gsl_matrix_set(cal2Invers, i, i, 1.0/gsl_matrix_get(cal2, i, i));
    }

    gsl_matrix *tmp = gsl_matrix_alloc(3, 3);
    gsl_matrix *x = gsl_matrix_alloc(3, 3);

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, measures, cal2Invers, 0.0, tmp); // Compute C = A B
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, cal1Invers, tmp, 0.0, x);

    // Compute the radius
    double sq = 0;

    for(int j = 0; j < 3; ++j){
        for(int i = 0; i < 3; ++i){
            sq += gsl_matrix_get(x, i, j)*gsl_matrix_get(x, i, j);
        }
    }

    double r = pow(6.0/sq, 1.0/6.0);

    // Compute position
    gsl_matrix *xTranspose = gsl_matrix_alloc(3, 3);
    gsl_matrix_transpose_memcpy(xTranspose, x);

    gsl_matrix *F = gsl_matrix_alloc(3, 3);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, xTranspose, x, 0.0, F);
    gsl_matrix_scale(F, pow(r, 6.0));

    double beta = asin(sqrt((gsl_matrix_get(F, 2, 2)-1)/3.0));
    double alpha = atan(gsl_matrix_get(F, 1, 2)/gsl_matrix_get(F, 0, 2));

    gsl_matrix *A = gsl_matrix_alloc(3, 3);
    gsl_matrix_set(A, 0, 0, 5);
    gsl_matrix_set(A, 1, 0, -1);
    gsl_matrix_set(A, 2, 0, -1);
    gsl_matrix_set(A, 0, 1, -1);
    gsl_matrix_set(A, 1, 1, 5);
    gsl_matrix_set(A, 2, 1, -1);
    gsl_matrix_set(A, 0, 2, -1);
    gsl_matrix_set(A, 1, 2, -1);
    gsl_matrix_set(A, 2, 2, 5);
    gsl_matrix_scale(A, pow(r, 8.0)/18.0);

    gsl_matrix *xSq = gsl_matrix_alloc(3, 1);
    gsl_matrix *dofSQ = gsl_matrix_alloc(3, 1);

    for(int j = 0; j < 3; ++j){
        sq = 0;

        for(int i = 0; i < 3; ++i){
            sq += gsl_matrix_get(x, i, j)*gsl_matrix_get(x, i, j);
        }

        gsl_matrix_set(xSq, j, 0, sq);
    }

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, xSq, 0.0, dofSQ); // Compute C = A B

    // Return
/*    gsl_matrix_set(position, 0, 0, r);
    gsl_matrix_set(position, 1, 0, alpha*180/M_PI);
    gsl_matrix_set(position, 2, 0, beta*180/M_PI);
*/

    (void) alpha;
    (void) beta;

    matrix_print(x);

    gsl_matrix_set(position, 0, 0, sqrt(gsl_matrix_get(dofSQ, 0, 0)));
    gsl_matrix_set(position, 1, 0, sqrt(gsl_matrix_get(dofSQ, 1, 0)));
    gsl_matrix_set(position, 2, 0, sqrt(gsl_matrix_get(dofSQ, 2, 0)));

    *octant = 0;

    gsl_matrix_memcpy(positionLast, position);
    gsl_matrix_memcpy(orientationLast, orientation);

    gsl_matrix_free(cal1Invers);
    gsl_matrix_free(cal2Invers);
    gsl_matrix_free(tmp);
    gsl_matrix_free(x);
    gsl_matrix_free(xTranspose);
    gsl_matrix_free(F);
    gsl_matrix_free(A);
    gsl_matrix_free(xSq);
    gsl_matrix_free(dofSQ);

    return 0;
}