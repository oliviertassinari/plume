#include "trajectory_simulation.h"

TrajectorySimulation::TrajectorySimulation(const double _rMin, const double _rMax, const double _originX, const double _originY, const double _outerSpeed1, const double _outerSpeed2, const double _innerSpeed, const double _voltageNoise, const double _frequency, const gsl_matrix * const _cal1, const gsl_matrix * const _cal2){
    rMin =_rMin;
    rMax = _rMax;
    originX = _originX;
    originY = _originY;
    outerSpeed1 = _outerSpeed1;
    outerSpeed2 = _outerSpeed2;
    innerSpeed = _innerSpeed;
    voltageNoise = _voltageNoise;
    period = 1.0/_frequency;
    elapsedTime = 0.0;

    cal1 = gsl_matrix_alloc(3,3);
    cal2 = gsl_matrix_alloc(3,3);

    gsl_matrix_memcpy(cal1,_cal1);
    gsl_matrix_memcpy(cal2,_cal2);

    srand(time(NULL));
}

TrajectorySimulation::~TrajectorySimulation(){
    gsl_matrix_free(cal1);
    gsl_matrix_free(cal2);
}

// Next simulated step
void TrajectorySimulation::next(gsl_matrix * const voltage_dst, gsl_matrix* const position_dst, gsl_matrix * const orientation_dst){
    double u;
    nextPosition(position_dst);
    nextOrientation(orientation_dst);
    // Computes voltages corresponding to the given position and orientation
    voltages_from_dof(voltage_dst,position_dst,orientation_dst,cal1,cal2);
    // Adds noises
    for(int i = 0;i<3;i++)
        for(int j = 0;j<3;j++){
            u = gsl_matrix_get(voltage_dst,i,j);
            gsl_matrix_set(voltage_dst,i,j,u+voltageNoise*noise());
        }
}

// Gives a gaussian normal reducted centered value
double TrajectorySimulation::noise(){
    // u and v are between 0 and 1
    double u = ((double) rand())/((double) RAND_MAX);
    double v = ((double) rand())/((double) RAND_MAX);
    double ans = sqrt(-2.0*log(u))*cos(2*M_PI*v);
    return ans;
}

void TrajectorySimulation::nextPosition(gsl_matrix *const dst){

    double x = originX + rMax*cos(0.1+outerSpeed2*elapsedTime);
    double y = originY + rMax*sin(0.1+outerSpeed2*elapsedTime);
    double z = 1;

    gsl_matrix_set(dst,0,0,x);
    gsl_matrix_set(dst,1,0,y);
    gsl_matrix_set(dst,2,0,z);

    // Increment time
    elapsedTime += period;
}

// Does nothing special at the moment
void TrajectorySimulation::nextOrientation(gsl_matrix *const dst){
    gsl_matrix_set(dst, 0, 0, 0); // Phi
    gsl_matrix_set(dst, 1, 0, 0); // Theta
    gsl_matrix_set(dst, 2, 0, 0); // Psi
}