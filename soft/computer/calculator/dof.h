#ifndef DOF_H
#define DOF_H

#include <stdint.h>

double dof(
    gsl_matrix *const position,
    gsl_matrix *const orientation,
    gsl_matrix *const measures,
    const gsl_matrix *const cal1,
    const gsl_matrix *const cal2,
    uint8_t *octant,
    gsl_matrix * const positionLast,
    gsl_matrix * const orientationLast,
    int signeRef[9]);

void voltages_from_dof(
    gsl_matrix *const voltages,
    const gsl_matrix *const position,
    const gsl_matrix *const orientation,
    const gsl_matrix *const cal1,
    const gsl_matrix *const cal2);

#endif