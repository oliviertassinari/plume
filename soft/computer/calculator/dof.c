#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include "matrix_utils.h"
#include "magnetic.h"
#include "debug_utils.h"

#include "dof.h"

// Computes a*b^-1
static void mulinv(gsl_matrix * const dst, const gsl_matrix * const a, const gsl_matrix * const b){
    DEBUG_ENTERING();
    gsl_matrix *invb=gsl_matrix_alloc(3,3);
    gsl_permutation *perm=gsl_permutation_alloc(3); 

    matrix_invert(invb,b);

    // C=A*B^-1
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,a,invb,0.0,dst);

    gsl_matrix_free(invb);
    gsl_permutation_free(perm);
    DEBUG_LEAVING();
}

// Computes the per-column norms
static void per_column_norms_squared(gsl_matrix * const dst, const gsl_matrix * const src){
    DEBUG_ENTERING();

    gsl_matrix *temp;
    double u;
    int i,j;

    temp=gsl_matrix_alloc(3,3);
    gsl_matrix_memcpy(temp,src);
    gsl_matrix_mul_elements(temp,src);
    // Computes the per-column squared norms and put them in dest
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

void voltages_from_dof(gsl_matrix *const voltages, const gsl_matrix *const position, const gsl_matrix *const orientation, const gsl_matrix *const cal1, const gsl_matrix *const cal2){
    DEBUG_ENTERING();

    gsl_matrix *thb=gsl_matrix_alloc(3,3);
    gsl_matrix *invcal=gsl_matrix_alloc(3,3);
    gsl_matrix *tmp=gsl_matrix_alloc(3,3);
    gsl_matrix *tmp2=gsl_matrix_alloc(3,3);

    // Computes the normalized field at given position
    theoric_normalized_field(thb,position);
    // Inverts the first calibration matrix
    matrix_invert(invcal,cal1);
    // Does the calculation
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,invcal,thb,0.0,tmp);
    // Invert the second calibration matrix
    matrix_invert(invcal,cal2);
    // Finishes the calculation
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,tmp,invcal,0.0,tmp2);

    double phi = gsl_matrix_get(orientation, 0, 0)*M_PI/180.0;
    double theta = gsl_matrix_get(orientation, 1, 0)*M_PI/180.0;
    double psi = gsl_matrix_get(orientation, 2, 0)*M_PI/180.0;

    gsl_matrix *rotation = gsl_matrix_alloc(3,3);

    gsl_matrix_set(rotation, 0, 0, cos(phi)*cos(theta));
    gsl_matrix_set(rotation, 0, 1, cos(phi)*sin(theta));
    gsl_matrix_set(rotation, 0, 2, -sin(phi));

    gsl_matrix_set(rotation, 1, 0, sin(psi)*sin(phi)*cos(theta)-cos(psi)*sin(theta));
    gsl_matrix_set(rotation, 1, 1, sin(psi)*sin(phi)*sin(theta)+cos(psi)*cos(theta));
    gsl_matrix_set(rotation, 1, 2, sin(psi)*cos(phi));

    gsl_matrix_set(rotation, 2, 0, cos(psi)*sin(phi)*cos(theta)+sin(psi)*sin(theta));
    gsl_matrix_set(rotation, 2, 1, cos(psi)*sin(phi)*sin(theta)-sin(psi)*cos(theta));
    gsl_matrix_set(rotation, 2, 2, cos(psi)*cos(phi));

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, rotation, tmp2, 0.0, voltages);

    gsl_matrix_free(rotation);
    gsl_matrix_free(thb);
    gsl_matrix_free(invcal);
    gsl_matrix_free(tmp);
    gsl_matrix_free(tmp2);
    DEBUG_LEAVING();
}

/**
 * Computes the position form the per-column norms
 * Give the answer in the first octant
 */
static void position_from_pcn(gsl_matrix * const dst, const gsl_matrix *const pcn){
    DEBUG_ENTERING();
    // Get the squared components
    double bxsq=pcn->data[0];
    double bysq=pcn->data[pcn->tda];
    double bzsq=pcn->data[2*pcn->tda];

    // Computes the radius
    double r=pow(6.0/(bxsq+bysq+bzsq),1.0/6.0);
    DEBUG_PRINT(("Radius: %e\n",r));
    // Computes the coordinates
    double u=pow(r,8.0);
    gsl_matrix_set(dst,0,0,sqrt(fabs((bxsq*u-r*r)/3)));
    gsl_matrix_set(dst,1,0,sqrt(fabs((bysq*u-r*r)/3)));
    gsl_matrix_set(dst,2,0,sqrt(fabs((bzsq*u-r*r)/3)));

    printf("radius %g\n", r);

    DEBUG_PRINT_MATRIX(("Position:\n",dst));
    DEBUG_LEAVING();
}

// Computes the position given the normalized field matrix
static void dof_position(gsl_matrix * const dst, const gsl_matrix * const nmf){ 
    DEBUG_ENTERING();
    gsl_matrix *pcn;
    // Compute the per-column norms
    pcn=gsl_matrix_alloc(3,1);
    per_column_norms_squared(pcn,nmf);
    // Comput the position using the pcn
    position_from_pcn(dst,pcn);

    // Free pcn
    gsl_matrix_free(pcn);
    DEBUG_LEAVING();
}

// Computes the nearest orthogonal matrix to m
static void nearest_orthogonal_matrix(gsl_matrix * const dst, const gsl_matrix * const m){
    DEBUG_ENTERING();

    gsl_matrix *u, *v;
    gsl_vector *s, *work;

    u=gsl_matrix_alloc(3,3);
    v=gsl_matrix_alloc(3,3);
    s=gsl_vector_alloc(3);
    work=gsl_vector_alloc(3);
    // copy m into u
    gsl_matrix_memcpy(u,m);

    // Singular values decomposition of m
    gsl_linalg_SV_decomp(u,v,s,work);

    // Computes the rotation matrix using the SVD
    // dst=u*trans(v)
    gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,u,v,0.0,dst);

    // frees
    gsl_matrix_free(u);
    gsl_matrix_free(v);
    gsl_vector_free(s);
    gsl_vector_free(work);
    DEBUG_PRINT_MATRIX(("Nearest orthgonal matrix:\n",dst));
    DEBUG_LEAVING();
}

// Computes the rotation matrix given the position and the nmf
static void dof_orientation(gsl_matrix *const dst, const gsl_matrix * const pos, const gsl_matrix * const nmf){
    DEBUG_ENTERING();
    gsl_matrix *thnmf;
    gsl_matrix *a;

    // Compute the theoric normalized field at the computed position
    thnmf=gsl_matrix_alloc(3,3);
    theoric_normalized_field(thnmf,pos);

    // Compute the product of the matrix with the inverse of an other
    a=gsl_matrix_alloc(3,3);
    mulinv(a,nmf,thnmf);

    // Compute the rotation matrix
    nearest_orthogonal_matrix(dst,a);

    // Free the matrices
    gsl_matrix_free(thnmf);
    gsl_matrix_free(a);
    DEBUG_LEAVING();
}

// Computes the position error given the position, orientation and normalized magnetic field
static double dof_error(const gsl_matrix * const pos, const gsl_matrix * const R, const gsl_matrix * const nmf){
    DEBUG_ENTERING();

    gsl_matrix *thnmf;
    gsl_matrix *temp;
    double ans=0.0;
    int i,j;

    // Compute the theoric normalized field at the computed position
    thnmf=gsl_matrix_alloc(3,3);
    theoric_normalized_field(thnmf,pos);

    DEBUG_PRINT(("nmf size: %zux%zu\n",nmf->size1,nmf->size2));
    // Compute the error
    temp=gsl_matrix_alloc(3,3);
    gsl_matrix_memcpy(temp,nmf);

    // Computes temp<- -1.0*R*thnmf + 1.0*temp
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,-1.0,R,thnmf,1.0,temp);

    // Square every element of the matrix
    gsl_matrix_mul_elements(temp,temp);
    // Add the values of the elements
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

// Computes the euler angles from the rotation matrix
static void mat_to_euler(gsl_matrix * const dst, const gsl_matrix * const R){
    DEBUG_ENTERING();
    double theta=180.0/M_PI*atan2(gsl_matrix_get(R,1,0),gsl_matrix_get(R,0,0)); // -180 : 180
    double psi=180.0/M_PI*atan2(gsl_matrix_get(R,2,1),gsl_matrix_get(R,2,2)); // -180 : 180
    double phi=180.0/M_PI*asin(gsl_matrix_get(R,2,0)); // -90 : 90

    gsl_matrix_set(dst,0,0,phi);
    gsl_matrix_set(dst,1,0,theta);
    gsl_matrix_set(dst,2,0,psi);
    DEBUG_PRINT_MATRIX(("Angles from rotation matrix:\n",dst));
    DEBUG_LEAVING();
}

// Computes the normalized field given the voltages and the calibration matrix
static void normalized_field(gsl_matrix * const dst, const gsl_matrix * const measures, const gsl_matrix * const cal1, const gsl_matrix * const cal2){
    DEBUG_ENTERING();
    gsl_matrix *temp=gsl_matrix_alloc(3,3);
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,cal1,measures,0.0,temp);
    gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,temp,cal2,0.0,dst);
    gsl_matrix_free(temp);
    DEBUG_PRINT_MATRIX(("Normalized magnetic field:\n",dst));
    DEBUG_LEAVING();
}

// Get the true position thanks to the octant
static void dof_position_octant(gsl_matrix * const position, uint8_t octant){
    for(int i = 0; i < 3; ++i){
        if((octant & (1 << i)) != 0){
            gsl_matrix_set(position, i, 0, -1*gsl_matrix_get(position, i, 0));
        }
    }
}

static double dofGetDistance(gsl_matrix * const position1, gsl_matrix * const orientation1, gsl_matrix * const position2, gsl_matrix * const orientation2){
    double distance = 0;

    for(int i = 0; i < 3; ++i){
        distance += abs(10*gsl_matrix_get(position1, i, 0) - 10*gsl_matrix_get(position2, i, 0));
        distance += abs(gsl_matrix_get(orientation1, i, 0) - gsl_matrix_get(orientation2, i, 0));
    }

    return distance;
}

/**
 * Computes the dof.
 * @param  octant (uint_8)
 *                bit 0 : axe X
 *                bit 1 : axe Y
 *                bit 2 : axe Z
 * @return (double)
 *         The error evaluation.
 */
double dof(gsl_matrix * const position, gsl_matrix * const orientation, gsl_matrix * const measures, const gsl_matrix *const cal1, const gsl_matrix * const cal2, uint8_t *octant, gsl_matrix * const positionLast, gsl_matrix * const orientationLast, int signeRef[9]){
    DEBUG_ENTERING(); 

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(signeRef[3*i+j] == -1){
                gsl_matrix_set(measures, i, j, -1*gsl_matrix_get(measures, i, j));
            }
        }
    }

    // Compute normalized magnetic field
    gsl_matrix *nmf = gsl_matrix_alloc(3,3);
    normalized_field(nmf, measures, cal1, cal2);

    // Compute the position in one octant
    dof_position(position, nmf);

    // Get the next octant
    if(gsl_matrix_get(position, 0, 0) < 0.5 || gsl_matrix_get(position, 1, 0) < 0.5 || gsl_matrix_get(position, 2, 0) < 0.5){
        gsl_matrix *positionToTry = gsl_matrix_alloc(3, 1);
        gsl_matrix *orientationToTry = gsl_matrix_alloc(3, 1);
        gsl_matrix *rotation2 = gsl_matrix_alloc(3,3);
        uint8_t octantToTry;
        double minDistance = -1;
        uint8_t newOctant = *octant;

        for(int i = 0; i < 8; ++i){
            gsl_matrix_memcpy(positionToTry, position);
            octantToTry = i;
            dof_position_octant(positionToTry, octantToTry);
            dof_orientation(rotation2, positionToTry, nmf);
            mat_to_euler(orientationToTry, rotation2);

            double distance = dofGetDistance(positionLast, orientationLast, positionToTry, orientationToTry);
            if(distance < minDistance || minDistance == -1){
                minDistance = distance;
                newOctant = i;
            }
        }

        gsl_matrix_free(positionToTry);
        gsl_matrix_free(orientationToTry);
        gsl_matrix_free(rotation2);
        (void)newOctant;
        //*octant = newOctant;
    }

    // Get the true position th anks to the octant
    dof_position_octant(position, *octant);

    // Compute the rotation matrix
    gsl_matrix * rotation = gsl_matrix_alloc(3,3);
    dof_orientation(rotation,position,nmf);

    // Compute the Euler angles from the rotation matrix
    mat_to_euler(orientation, rotation);

    gsl_matrix_memcpy(positionLast, position);
    gsl_matrix_memcpy(orientationLast, orientation);

    // Compute error
    double e = dof_error(position,rotation,nmf);

    gsl_matrix_free(nmf);
    gsl_matrix_free(rotation);
    return e;
    DEBUG_LEAVING();
}
