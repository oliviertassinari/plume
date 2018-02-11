#include "PlumeReceiver.h"

#define BUFFER_SIZE     (1<<BUFFER_DEPTH)
#define BUFFER_DEPTH    7
#define DEGREE          2 //Quadratic approximation

/*Matrix freeing Callback*/
void cb_free_matrix(void *m){
        DEBUG_ENTERING();
        gsl_matrix_free((gsl_matrix*)m);
        DEBUG_LEAVING();
}

/*Constructor: needs a pointer to the emmiter and the receiver ble id*/
PlumeReceiver::PlumeReceiver(PlumeEmitter *_emmiter, const string _id, const gsl_matrix * const _cal){
        DEBUG_ENTERING();
        /*First allocate the matrices*/
        cal=gsl_matrix_alloc(3,3);
        filteredPosition=gsl_matrix_alloc(3,1);
        filteredOrientation=gsl_matrix_alloc(3,1);
        /*matrices containing the polynom coefficient for DOF estimation*/
        positionPolynom=gsl_matrix_alloc(3,DEGREE+1);
        orientationPolynom=gsl_matrix_alloc(3,DEGREE+1);

        /*Set first octant*/
        nextOctant=0;

        /*Initialize the mutexes*/
        pthread_mutex_init(&voltageMutex,NULL);
        pthread_mutex_init(&DOFMutex,NULL);

        /*Initialize the circular buffers*/
        positionBuffer=new CircularBuffer(BUFFER_DEPTH,cb_free_matrix);
        orientationBuffer=new CircularBuffer(BUFFER_DEPTH,cb_free_matrix);

        /*finally copy args*/
        gsl_matrix_memcpy(cal,_cal);
        id=_id;
        emitter=_emmiter;

        DEBUG_LEAVING();
}

PlumeReceiver::~PlumeReceiver(){
        DEBUG_ENTERING();
        gsl_matrix_free(cal);
        gsl_matrix_free(filteredPosition);
        gsl_matrix_free(filteredOrientation);
        gsl_matrix_free(positionPolynom);
        gsl_matrix_free(orientationPolynom);

        pthread_mutex_destroy(&voltageMutex); 
        pthread_mutex_destroy(&DOFMutex);

        delete positionBuffer;
        delete orientationBuffer;
        DEBUG_LEAVING();
}

/*Computes the dof*/
void PlumeReceiver::compute6DOF(const gsl_matrix * const voltages){
        DEBUG_ENTERING();
        gsl_matrix *tmp=gsl_matrix_alloc(3,3);
        
        emitter->getCalib(tmp);
        /*Maybe we will need to reverse cal and tmp*/
        dof(position, orientation, voltages, cal, tmp,nextOctant);
        
        gsl_matrix_free(tmp);
        DEBUG_LEAVING();
}

/*treat the given voltages, updates class accordingly*/
void PlumeReceiver::treatVoltages(const gsl_matrix * const voltages){
        DEBUG_ENTERING();
        pthread_mutex_lock(&voltageMutex);
        pthread_mutex_lock(&DOFMutex);

        /*They are automatically freed by the buffer*/
        position=gsl_matrix_alloc(3,1);
        orientation=gsl_matrix_alloc(3,1);

        /*Computes position and orientation*/
        compute6DOF(voltages);  
        /*the buffers automatically free the overwritten data*/
        positionBuffer->push(position);
        orientationBuffer->push(orientation);
        /*Computes the polynom if there are enough values in buffer*/
        if(positionBuffer->getSize()>DEGREE){ 
                compute6DOFPolynoms();
                computeFiltered6DOF();
                computeNextOctant();
        }else{ /*It is not filtered*/
                gsl_matrix_memcpy(filteredPosition,position);
                gsl_matrix_memcpy(filteredOrientation,orientation);
        }
        pthread_mutex_unlock(&DOFMutex);
        pthread_mutex_unlock(&voltageMutex);
        DEBUG_LEAVING();
}

/*Return the DOF without filtering*/
void PlumeReceiver::getRaw6DOF(gsl_matrix * const position_dst, gsl_matrix * const orientation_dst){
        DEBUG_ENTERING();
        pthread_mutex_lock(&DOFMutex);

        gsl_matrix_memcpy(position_dst,(gsl_matrix*)positionBuffer->get(0));
        gsl_matrix_memcpy(orientation_dst,(gsl_matrix*)positionBuffer->get(0));

        pthread_mutex_unlock(&DOFMutex);
        DEBUG_LEAVING();
}

/*Return the filtered DOF which was coputed using the estimation polynoms*/
void PlumeReceiver::getFiltered6DOF(gsl_matrix * const position_dst, gsl_matrix * const orientation_dst){
        DEBUG_ENTERING();
        pthread_mutex_lock(&DOFMutex);

        gsl_matrix_memcpy(position_dst,filteredPosition);
        gsl_matrix_memcpy(orientation_dst,filteredOrientation);

        pthread_mutex_unlock(&DOFMutex);
        DEBUG_LEAVING();
}

/*estimate the position at given moment using the estiamtion polynoms*/
void PlumeReceiver::estimatePosition(gsl_matrix * const position_dst, gsl_matrix * const orientation_dst, const double secs){
        DEBUG_ENTERING();
        int i;
        double a=1.0;
        gsl_matrix *t=gsl_matrix_alloc(DEGREE+1,1);

        /*put secs^i in t*/
        for(i=0;i<DEGREE+1;i++){
                gsl_matrix_set(t,i,0,a);
                a*=secs;
        }
        /*Computes the estimated position*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,positionPolynom,t,0.0,position_dst);
        /*Computes the estimated orientation*/
        gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,orientationPolynom,t,0.0,orientation_dst);
        gsl_matrix_free(t);    
        DEBUG_LEAVING();
}

/*Computes the dof estimation polynoms*/
void PlumeReceiver::compute6DOFPolynoms(){
        DEBUG_ENTERING();
        int i,j;
        int n=positionBuffer->getSize();
        gsl_matrix *tmp=gsl_matrix_alloc(DEGREE+1,1);
        /*time matrix*/
        gsl_matrix *t=gsl_matrix_alloc(n,1);
        /*position matrix*/
        gsl_matrix *p=gsl_matrix_alloc(n,1);

        /*Set times*/
        for(j=0;j<n;j++){
                gsl_matrix_set(t,j,0,-((double)j)/FREQUENCY);
        }
        /*Compute position polynom*/
        for(i=0;i<3;i++){
                /*set coordinates vector*/
                for(j=0;j<n;j++){
                        gsl_matrix_set(p,j,0,gsl_matrix_get((gsl_matrix*)positionBuffer->get(j),i,0));
                }
                /*Compute the coefficients for this coordinate*/
                polynomial_regression(tmp,t,p);
                /*Copy values in attribute*/
                for(j=0;j<DEGREE+1;j++){
                        gsl_matrix_set(positionPolynom,i,j,gsl_matrix_get(tmp,j,0));
                }
        }

        /*Compute orientation polynom*/
        for(i=0;i<3;i++){
                /*set coordinates vector*/
                for(j=0;j<n;j++){
                        gsl_matrix_set(p,j,0,gsl_matrix_get((gsl_matrix*)orientationBuffer->get(j),i,0));
                }
                /*Compute the coefficients for this coordinate*/
                polynomial_regression(tmp,t,p);
                /*Copy values*/
                for(j=0;j<DEGREE+1;j++){
                        gsl_matrix_set(orientationPolynom,i,j,gsl_matrix_get(tmp,j,0));
                }
        }

        /*Free*/
        gsl_matrix_free(tmp);
        gsl_matrix_free(t);
        gsl_matrix_free(p);
        DEBUG_PRINT_MATRIX(("Position polynom:\n",positionPolynom));
        DEBUG_LEAVING();
}

/*Compute the filtered DOF*/
void PlumeReceiver::computeFiltered6DOF(){
        DEBUG_ENTERING();
        estimatePosition(filteredPosition,filteredOrientation,0.0);
        DEBUG_LEAVING();
}

/*Try to estimate the next octant*/
void PlumeReceiver::computeNextOctant(){
        DEBUG_ENTERING();
        gsl_matrix *nextP=gsl_matrix_alloc(3,1);
        gsl_matrix *nextO=gsl_matrix_alloc(3,1);

        /*Computes future DOF*/
        estimatePosition(nextP,nextO,1.0/FREQUENCY);

        nextOctant=0;
        nextOctant|=(gsl_matrix_get(nextP,0,0)<0.0)?1:0;   //X sign will be -
        nextOctant|=(gsl_matrix_get(nextP,1,0)<0.0)?2:0;   //Y sign will be -
        nextOctant|=(gsl_matrix_get(nextP,2,0)<0.0)?4:0;   //Z sign will be -

        gsl_matrix_free(nextP);
        gsl_matrix_free(nextO);
        DEBUG_LEAVING();
}

uint8_t PlumeReceiver::getNextOctant() const{
        return nextOctant;
}
