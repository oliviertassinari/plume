#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#ifdef DEBUG
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <gsl/gsl_matrix.h>
#endif

#ifdef DEBUG
#define DEBUG_PRINT(x) printf("###### ");printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#ifdef DEBUG
#define DEBUG_ENTERING() printf(">>>>>> Entering %s\n",__func__)
#else
#define DEBUG_ENTERING() do {} while (0)
#endif

#ifdef DEBUG
#define DEBUG_LEAVING() printf("<<<<<< Leaving %s\n",__func__)
#else
#define DEBUG_LEAVING() do {} while (0)
#endif

#ifdef DEBUG
#define DEBUG_PRINT_MATRIX(x) debug_print_matrix x
#else
#define DEBUG_PRINT_MATRIX(x) do {} while (0)
#endif

#ifdef DEBUG
void debug_print_matrix(const char * const str, const gsl_matrix * const m);
#endif

#endif
