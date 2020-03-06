#include <iostream>
#include <cmath>
#include "metric.hpp"
#include "photon.hpp"
#include "disk.hpp"

//This is a function that will take in the initial position vector (posVec) and momentum vector (momVec) and integrate them forward, until rLimit or disk is reached, or it dies after so many steps

#ifndef INTEGRATOR_H // include guard
#define INTEGRATOR_H

class Integrator {
    double stepSize, tolerance, maxNumSteps, rMin;

    public:
        Integrator(Metric metric, double newStepSize, double newTolerance, double newMaxStep, double horizonStop);

        void checkSwitches(Photon photon, double kRsq, double kThSq);

        void refineStepSize(Photon photon, Metric metric, double kR, double kTh, double kPhi);

        bool hasHitDisk(Photon photon, Disk disk);

        void propagate(Metric metric, Photon photon, Disk disk);  
};
#endif //INTEGRATOR_H