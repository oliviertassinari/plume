#ifndef MAGNETIC_H
#define MAGNETIC_H

#include <gsl/gsl_blas.h>

/**
 * Computess the theoric normalized magnetic field given the position
 * @param dst (gsl_matrix*)
 *            Magnetic field computed.
 * @param pos (gsl_matrix*)
 *            Position in witch we compute the magnetic field.
 */
void theoric_normalized_field(gsl_matrix *const dst, const gsl_matrix *const pos);

#endif
