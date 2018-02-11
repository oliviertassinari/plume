#include "TrajectorySimulation.h"

#define PI 3.1415926535897932384626433832795
//start angles
#define A1 1.0
#define A2 1.0
#define A3 1.0


TrajectorySimulation::TrajectorySimulation(const double _rMin, const double _rMax, const double _outerSpeed1, const double _outerSpeed2, const double _innerSpeed, const double _voltageNoise, const double _frequency, const gsl_matrix * const _cal1, const gsl_matrix * const _cal2){
        DEBUG_ENTERING();
        rMin=_rMin;
        rMax=_rMax;
        outerSpeed1=_outerSpeed1;
        outerSpeed2=_outerSpeed2;
        innerSpeed=_innerSpeed;
        voltageNoise=_voltageNoise;
        period=1.0/_frequency;
        elapsedTime=0.0;

        cal1=gsl_matrix_alloc(3,3);
        cal2=gsl_matrix_alloc(3,3);

        gsl_matrix_memcpy(cal1,_cal1);
        gsl_matrix_memcpy(cal2,_cal2);

        srand(time(NULL));
        DEBUG_LEAVING();
}

TrajectorySimulation::~TrajectorySimulation(){
        gsl_matrix_free(cal1);
        gsl_matrix_free(cal2);
}

/*Next simulated step*/
void TrajectorySimulation::next(gsl_matrix * const voltage_dst, gsl_matrix* const position_dst, gsl_matrix * const orientation_dst){
        DEBUG_ENTERING();
        double u;
        nextPosition(position_dst);
        nextOrientation(orientation_dst);
        /*Computes voltages corresponding to the given position and orientation*/
        voltages_from_dof(voltage_dst,position_dst,orientation_dst,cal1,cal2);
        /*Adds noises*/
        for(int i=0;i<3;i++)
                for(int j=0;j<3;j++){
                        u=gsl_matrix_get(voltage_dst,i,j);
                        gsl_matrix_set(voltage_dst,i,j,u+voltageNoise*noise());
                }
        DEBUG_PRINT_MATRIX(("Voltages with noise:\n",voltage_dst));
        DEBUG_LEAVING();
}

/*Gives a gaussian normal reducted centered value*/
double TrajectorySimulation::noise(){
        DEBUG_ENTERING();
        /*u and v are between 0 and 1*/
        double u=((double) rand())/((double) RAND_MAX);
        double v=((double) rand())/((double) RAND_MAX);
        double ans=sqrt(-2.0*log(u))*cos(2*PI*v);
        DEBUG_PRINT(("Noise value: %e\n",ans));
        DEBUG_LEAVING();
        return ans;
}

void TrajectorySimulation::nextPosition(gsl_matrix *const dst){
        DEBUG_ENTERING();
        double r1=(rMax+rMin)/2.0;
        double r2=(rMax-rMin)/2.0;
        /*big sphere*/
        double x=r1*cos(A1+outerSpeed1*elapsedTime)*cos(A2+outerSpeed2*elapsedTime);
        double y=r1*cos(A1+outerSpeed1*elapsedTime)*sin(A2+outerSpeed2*elapsedTime);
        double z=r1*sin(A1+outerSpeed1*elapsedTime);
        /*little sphere*/
        x+=r2*cos(A3+innerSpeed*elapsedTime);
        y+=r2*sin(A3+innerSpeed*elapsedTime);

        gsl_matrix_set(dst,0,0,x);
        gsl_matrix_set(dst,1,0,y);
        gsl_matrix_set(dst,2,0,z);
        /*Increment time*/
        elapsedTime+=period;
        DEBUG_PRINT_MATRIX(("Next position:\n",dst));
        DEBUG_LEAVING();
}

/*Does nothing special at the moment*/
void TrajectorySimulation::nextOrientation(gsl_matrix *const dst){
        DEBUG_ENTERING();
        for(int i=0;i<3;i++)
                gsl_matrix_set(dst,i,0,0.0);
        DEBUG_LEAVING();
}
