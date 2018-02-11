#ifndef TRAJECTORY_SIMULATION_H
#define TRAJECTORY_SIMULATION_H

#include <iostream>
#include <math.h>
#include <gsl/gsl_matrix.h>

extern "C"{
    #include "dof.h"
}

class TrajectorySimulation{
    public:
        TrajectorySimulation(
                            const double _rMin,
                            const double _rMax,
                            const double _originX,
                            const double _originY,
                            const double _outerSpeed1,
                            const double _outerSpeed2,
                            const double _innerSpeed,
                            const double _voltageNoise,
                            const double _frequency,
                            const gsl_matrix * const cal1,
                            const gsl_matrix * const cal2);
        ~TrajectorySimulation();

        void next(gsl_matrix * const voltage_dst, gsl_matrix* const position_dst, gsl_matrix * const orientation_dst);

    private:
        void nextPosition(gsl_matrix * const dst);
        void nextOrientation(gsl_matrix * const dst);
        double noise();
        double rMin;
        double rMax;
        double originX;
        double originY;
        double outerSpeed1;
        double outerSpeed2;
        double innerSpeed;
        double voltageNoise;
        double period;
        double elapsedTime;
        gsl_matrix *cal1;
        gsl_matrix *cal2;
};

#endif