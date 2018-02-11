#include "dof.h"

#define PI 3.1415926535897932384626433832795

/*Computes the normalized field given the voltages and the calibration matrix*/
static void normalized_field(gsl_matrix * const dst, const gsl_matrix * const measures, const gsl_matrix * const cal1, const gsl_matrix * const cal2);
/*Computes the position given the normalized field matrix*/
static void dof_position(gsl_matrix * const dst, const gsl_matrix * const nmf,const uint8_t octant);
/*Computes the rotation matrix given the position and the nmf*/
static void dof_orientation(gsl_matrix * const dst,const gsl_matrix * const pos, const gsl_matrix * const nmf);
/*Computes the position error given the position, orientation and normalized magnetic field*/
static double dof_error(const gsl_matrix * const pos, const gsl_matrix *const R, const gsl_matrix * const nmf);
/*Computes the per-column norms*/
static void per_column_norms_squared(gsl_matrix * const dst, const gsl_matrix * const src);
/*Computes the position form the per-column norms*/
/*Give the answer in the first octant*/
static void position_from_pcn(gsl_matrix * const dst, const gsl_matrix *const pcn, const uint8_t octant);
/*Computes a*b^-1*/
static void mulinv(gsl_matrix * const dst, const gsl_matrix * const a, const gsl_matrix * const b);
/*Computes the euler angles from the rotation matrix*/
static void mat_to_euler(gsl_matrix * const dst, const gsl_matrix * const R);
/*Computes the nearest orthogonal matrix to m*/
static void nearest_orthogonal_matrix(gsl_matrix * const dst, const gsl_matrix * const m);


/*Computes the dof*/
/*Returns the error evaluation*/
double dof(gsl_matrix * const position, gsl_matrix * const orientation, const gsl_matrix * const measures, const gsl_matrix *const cal1, const gsl_matrix * const cal2, const uint8_t octant){
        DEBUG_ENTERING(); 
        /*Normalized magnetic field*/
        gsl_matrix *nmf;
        gsl_matrix *R;
        double e;

        /*Compute normalized magnetic field*/
        nmf=gsl_matrix_alloc(3,3);
        normalized_field(nmf,measures,cal1,cal2);

        /*Compute the position*/
        dof_position(position,nmf,octant);

        /*Compute the rotation matrix*/
        R=gsl_matrix_alloc(3,3);
        dof_orientation(R,position,nmf);

        /*Compute the Euler angles from the rotation matrix*/
        mat_to_euler(orientation,R);

        /*Compute error*/
        e=dof_error(position,R,nmf);

        gsl_matrix_free(nmf);
        gsl_matrix_free(R);
        return e;
        DEBUG_LEAVING();
}

/*Doesn't use orientation yet*/
void voltages_from_dof(gsl_matrix *const voltages, const gsl_matrix *const position, const gsl_matrix *const orientation, const gsl_matrix *const cal1, const gsl_matrix *const cal2){
        DEBUG_ENTERING();
        (void) orientation;
        gsl_matrix *thb=gsl_matrix_alloc(3,3);
        gsl_matrix *invcal=gsl_matrix_alloc(3,3);
        gsl_matrix *tmp=gsl_matrix_alloc(3,3);

        /*Computes the normalized field at given position*/
        theoric_normalized_field(thb,position);
        /*Inverts the first calibration matrix*/
        matrix_invert(invcal,cal1);
        /*Does the calculation*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,invcal,thb,0.0,tmp);
        /*Invert the second calibration matrix*/
        matrix_invert(invcal,cal2);
        /*Finishes the calculation*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp,invcal,0.0,voltages);

        gsl_matrix_free(thb);
        gsl_matrix_free(invcal);
        gsl_matrix_free(tmp);
        DEBUG_LEAVING();
}

static void dof_position(gsl_matrix * const dst, const gsl_matrix * const nmf, const uint8_t octant){ 
        DEBUG_ENTERING();
        gsl_matrix *pcn;
        /*Compute the per-column norms*/
        pcn=gsl_matrix_alloc(3,1);
        per_column_norms_squared(pcn,nmf);
        /*Comput the position using the pcn*/
        position_from_pcn(dst,pcn,octant);

        /*Free pcn*/
        gsl_matrix_free(pcn);
        DEBUG_LEAVING();
}

static void dof_orientation(gsl_matrix *const dst, const gsl_matrix * const pos, const gsl_matrix * const nmf){
        DEBUG_ENTERING();
        gsl_matrix *thnmf;
        gsl_matrix *a;

        /*Compute the theoric normalized field at the computed position*/
        thnmf=gsl_matrix_alloc(3,3);
        theoric_normalized_field(thnmf,pos);

        /*Compute the product of the matrix with the inverse of an other*/
        a=gsl_matrix_alloc(3,3);
        mulinv(a,nmf,thnmf);

        /*Compute the rotation matrix*/
        nearest_orthogonal_matrix(dst,a);

        /*Free the matrices*/
        gsl_matrix_free(thnmf);
        gsl_matrix_free(a);
        DEBUG_LEAVING();
}

static double dof_error(const gsl_matrix * const pos, const gsl_matrix * const R, const gsl_matrix * const nmf){
        DEBUG_ENTERING();

        gsl_matrix *thnmf;
        gsl_matrix *temp;
        double ans=0.0;
        int i,j;

        /*Compute the theoric normalized field at the computed position*/
        thnmf=gsl_matrix_alloc(3,3);
        theoric_normalized_field(thnmf,pos);

        DEBUG_PRINT(("nmf size: %zux%zu\n",nmf->size1,nmf->size2));
        /*Compute the error*/
        temp=gsl_matrix_alloc(3,3);
        gsl_matrix_memcpy(temp,nmf);

        /*Computes temp<- -1.0*R*thnmf + 1.0*temp*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,-1.0,R,thnmf,1.0,temp);

        /*Square every element of the matrix*/
        gsl_matrix_mul_elements(temp,temp);
        /*add the values of the elements*/
        for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                        ans+=temp->data[i*temp->tda+j];

        gsl_matrix_free(temp);
        gsl_matrix_free(thnmf);
        ans=sqrt(ans);
        DEBUG_PRINT(("%e\n",ans));    
        DEBUG_LEAVING();
        return ans;

}

static void per_column_norms_squared(gsl_matrix * const dst, const gsl_matrix * const src){
        DEBUG_ENTERING();

        gsl_matrix *temp;
        double u;
        int i,j;

        temp=gsl_matrix_alloc(3,3);
        gsl_matrix_memcpy(temp,src);
        gsl_matrix_mul_elements(temp,src);
        /*Computes the per-column squared norms and put them in dest*/
        for(i=0;i<3;i++){
                u=0.0;
                for(j=0;j<3;j++)
                        u+=temp->data[j*temp->tda+i];
                dst->data[i*dst->tda]=u;
        }
        gsl_matrix_free(temp);
        DEBUG_PRINT_MATRIX(("Per column norms squared:\n",dst));
        DEBUG_LEAVING();
}


static void position_from_pcn(gsl_matrix * const dst, const gsl_matrix *const pcn, const uint8_t octant){
        DEBUG_ENTERING();
        /*Get the squared components*/
        double bxsq=pcn->data[0];
        double bysq=pcn->data[pcn->tda];
        double bzsq=pcn->data[2*pcn->tda];

        /*Computes the radius*/
        double r=pow(6.0/(bxsq+bysq+bzsq),1.0/6.0);
        DEBUG_PRINT(("Radius: %e\n",r));
        /*Computes the coordinates*/
        double u=pow(r,8.0);
        gsl_matrix_set(dst,0,0,sqrt(fabs((bxsq*u-r*r)/3)));
        gsl_matrix_set(dst,1,0,sqrt(fabs((bysq*u-r*r)/3)));
        gsl_matrix_set(dst,2,0,sqrt(fabs((bzsq*u-r*r)/3)));

        /*Adjusts signs according to octant*/
        dst->data[0]*=(octant&1)?-1.0:1.0;
        dst->data[dst->tda]*=(octant&2)?-1.0:1.0;
        dst->data[2*dst->tda]*=(octant&4)?-1.0:1.0;

        DEBUG_PRINT_MATRIX(("Position:\n",dst));
        DEBUG_LEAVING();
}



static void mulinv(gsl_matrix * const dst, const gsl_matrix * const a, const gsl_matrix * const b){
        DEBUG_ENTERING();
        gsl_matrix *invb=gsl_matrix_alloc(3,3);
        gsl_permutation *perm=gsl_permutation_alloc(3); 

        matrix_invert(invb,b);

        /*C=A*B^-1*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,a,invb,0.0,dst);

        gsl_matrix_free(invb);
        gsl_permutation_free(perm);
        DEBUG_LEAVING();
}


static void mat_to_euler(gsl_matrix * const dst, const gsl_matrix * const R){
        DEBUG_ENTERING();
        double theta=180.0/PI*atan2(gsl_matrix_get(R,1,0),gsl_matrix_get(R,0,0));
        double phi=180.0/PI*sin(gsl_matrix_get(R,2,0));
        double psi=180.0/PI*atan2(gsl_matrix_get(R,2,1),gsl_matrix_get(R,2,2));

        gsl_matrix_set(dst,0,0,phi);
        gsl_matrix_set(dst,1,0,theta);
        gsl_matrix_set(dst,2,0,psi);
        DEBUG_PRINT_MATRIX(("Angles from rotation matrix:\n",dst));
        DEBUG_LEAVING();
}

static void nearest_orthogonal_matrix(gsl_matrix * const dst, const gsl_matrix * const m){
        DEBUG_ENTERING();

        gsl_matrix *u, *v;
        gsl_vector *s, *work;

        u=gsl_matrix_alloc(3,3);
        v=gsl_matrix_alloc(3,3);
        s=gsl_vector_alloc(3);
        work=gsl_vector_alloc(3);
        /*copy m into u*/
        gsl_matrix_memcpy(u,m);

        /*Singular values decomposition of m*/
        gsl_linalg_SV_decomp(u,v,s,work);

        /*Computes the rotation matrix using the SVD*/
        /*dst=u*trans(v)*/
        gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,u,v,0.0,dst);

        /*frees*/
        gsl_matrix_free(u);
        gsl_matrix_free(v);
        gsl_vector_free(s);
        gsl_vector_free(work);
        DEBUG_PRINT_MATRIX(("Nearest orthgonal matrix:\n",dst));
        DEBUG_LEAVING();
}

static void normalized_field(gsl_matrix * const dst, const gsl_matrix * const measures, const gsl_matrix * const cal1, const gsl_matrix * const cal2){
        DEBUG_ENTERING();
        gsl_matrix *temp=gsl_matrix_alloc(3,3);
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,cal1,measures,0.0,temp);
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,temp,cal2,0.0,dst);
        gsl_matrix_free(temp);
        DEBUG_PRINT_MATRIX(("Normalized magnetic field:\n",dst));
        DEBUG_LEAVING();
}

