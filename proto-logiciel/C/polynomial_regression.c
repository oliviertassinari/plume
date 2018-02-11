#include "polynomial_regression.h"

//See http://www.stat.purdue.edu/~jennings/stat514/stat512notes/topic3.pdf*/
//Computes polynom with degree = number of columns in ans
void polynomial_regression(gsl_matrix * const ans, const gsl_matrix * const x, const gsl_matrix * const y){
        DEBUG_ENTERING();
        int i,j;
        double p;
        /*Degree of the regression*/
        int n=ans->size1;
        /*Number of measures*/
        int m=y->size1;
        /*n*n matrics*/
        gsl_matrix *a=gsl_matrix_alloc(n,n);
        gsl_matrix *b=gsl_matrix_alloc(n,m);
        gsl_matrix *inva=gsl_matrix_alloc(n,n);
        gsl_matrix *u=gsl_matrix_alloc(m,n);

        /*Fill u*/
        for(i=0;i<m;i++){
                p=1;
                for(j=0;j<n;j++){
                        gsl_matrix_set(u,i,j,p);
                        p*=gsl_matrix_get(x,i,0);
                }
        }

        /*a=trans(x)*x*/
        gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,u,u,0.0,a);
        /*invert a*/
        matrix_invert(inva,a);
        /*b=inva*trans(x)*/
        DEBUG_PRINT(("size of inva: %zux%zu\n",inva->size1,inva->size2));
        gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,inva,u,0.0,b);
        /*ans=b*y*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,b,y,0.0,ans);

        /*Free*/
        gsl_matrix_free(a);
        gsl_matrix_free(inva);
        gsl_matrix_free(b);
        gsl_matrix_free(u);
        DEBUG_PRINT_MATRIX(("Obtained polynomial regression coefficients:\n",ans));
        DEBUG_LEAVING();
}
