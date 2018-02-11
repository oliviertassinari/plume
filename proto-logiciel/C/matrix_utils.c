#include "matrix_utils.h"

void matrix_invert(gsl_matrix * const dst, const gsl_matrix * const src){
        int s;
        gsl_permutation *p = gsl_permutation_alloc(src->size1);

        /*Copies input matrix*/
        gsl_matrix *tmp=gsl_matrix_alloc(src->size1,src->size2);
        gsl_matrix_memcpy(tmp,src);

        /*Invert*/
        gsl_linalg_LU_decomp(tmp,p,&s);
        gsl_linalg_LU_invert(tmp,p,dst);

        gsl_permutation_free(p);
        gsl_matrix_free(tmp);

}

/*Prints nicely a matrix*/
void matrix_print(const gsl_matrix *const m){
        uint32_t i,j;	
        for(i=0;i<m->size1;i++){
                printf("|\t");
                for(j=0;j<m->size2;j++){
                        printf("%e\t",gsl_matrix_get(m,i,j));
                }
                printf("|\n");
        }
}
