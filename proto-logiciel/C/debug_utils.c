#ifdef DEBUG
#include "debug_utils.h"

/*Prints a matrix and a text*/
void debug_print_matrix(const char * const str, const gsl_matrix * const m){
        uint32_t i,j;
        printf("###### ");
        printf(str);
        for(i=0;i<m->size1;i++){
                printf("###### |\t");
                for(j=0;j<m->size2;j++){
                        printf("%e\t",gsl_matrix_get(m,i,j));
                }
                printf("|\n");
        }
}
#endif
