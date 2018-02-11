#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <gsl/gsl_blas.h>

/**
 * Compute the invert of a matrix.
 * @param dst (gsl_matrix*)
 *            Invert of the matrix.
 * @param src (gsl_matrix*)
 *            Matrix to invert.
 */
void matrix_invert(gsl_matrix *const dst, const gsl_matrix *const src);

/**
 * Prints a matrix nicely.
 * @param m (gsl_matrix*)
 *          Matrix to print.
 */
void matrix_print(const gsl_matrix *const m);

#endif
