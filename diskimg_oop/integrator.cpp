#include <iostream>
#include <cmath>
#include "metric.hpp"
#include "photon.hpp"
#include "disk.hpp"
#include "integrator.hpp"

//This is a function that will take in the initial position vector (posVec) and momentum vector (momVec) and integrate them forward, until rLimit or disk is reached, or it dies after so many steps

Integrator::Integrator(Metric metric, double newTolerance, double newNumMaxSteps, double horizonStop) {
    tolerance = newTolerance;
    maxNumSteps = newNumMaxSteps;
    rMin = metric.eventHorizon() + horizonStop;
}

void Integrator::checkSwitches(Photon photon, double kRsq, double kThSq) {
    //Checking if rdot^2 is at a minimal value at 0, in which case, the sign of rdot must be flipped
    if (kRsq < 0.){
        photon.updateRswitch(-1.*photon.getRswitch());  //flipping the sign of rSqrtSwitch
    }
    //Checking if thetadot^2 is at a minimal value at 0, in which case, the sign of thetadot must be flipped
    if (kThSq < 0.){
        photon.updateThetaSwitch(-1.*photon.getThetaSwitch());  //flipping the sign of thSqrtSwitch
    }
}

void Integrator::refineStepSize(Photon photon, Metric metric, double kR, double kTh, double kPhi) {
    //Calculating step size (auto-refinement)
    stepSize = ((photon.radius() - metric.eventHorizon())/(std::fabs(kR)*tolerance));
    if (stepSize > (1./(std::fabs(kTh*tolerance)))){
        stepSize = 1./(std::fabs(kTh)*tolerance);
    }
    if (stepSize > (1./(std::fabs(kPhi*tolerance)))){
        stepSize = 1./(std::fabs(kPhi)*tolerance);
    }
    stepSize = -1.*stepSize;
}

bool Integrator::hasHitDisk(Photon photon, Disk disk) {
    double position[4] = {photon.time(), photon.radius(), photon.theta(), photon.phi()};
    return (verticalHeight(position) <= disk.scaleHeight(position));
}

void Integrator::propagate(Metric metric, Photon photon, Disk disk){

    //Defining arrays that will hold the new photon position
    double newPosition[4];
    double newMomentum[4];

    //Defining the RK4 test position vector
    double testPosition[4];

    //Defining k-arrays, each entry a seperate term for the RK4 integration
    double kTarray[4];
    double kRsqArray[4];
    double kRarray[4];
    double kThSqArray[4];
    double kThArray[4];
    double kPhiArray[4];

    //Defining step to be taken per step
    double tStep;
    double rStep;
    double thStep;
    double phiStep;

    //defining integration counter i
	int i = 0;

    //starting main processing loop.  Will continue while i is less than the maximum number of steps and the radial distance is less than the minimal value specified
	while ((i < maxNumSteps) && (photon.radius() > rMin) && (!hasHitDisk(photon, disk))) {
        
        //Setting first test position
        testPosition[0] = photon.time();
        testPosition[1] = photon.radius();
        testPosition[2] = photon.theta();
        testPosition[3] = photon.phi();
        
        //First set of k's for rk4 integration
        kTarray[0] = photon.tdot(metric, testPosition);
        kRsqArray[0] = photon.rdotsq(metric, testPosition);
        kThSqArray[0] = photon.thdotsq(metric, testPosition);
        kPhiArray[0] = photon.phidot(metric, testPosition);

        //Checking switches for r^2 and theta^2
        checkSwitches(photon, kRsqArray[0], kThSqArray[0]);

        //Taking sqrt of r^2 and theta^2, setting sign to be consistent with switches
        kRarray[0] = photon.getRswitch()*sqrt(std::fabs(kRsqArray[0]));
        kThArray[0] = photon.getThetaSwitch()*sqrt(std::fabs(kThSqArray[0]));

        std::cout << kTarray[0] << " " << kRsqArray[0] << " " << kThSqArray[0] << " " << kPhiArray[0] << "\n";

        refineStepSize(photon, metric, kRarray[0], kThArray[0], kPhiArray[0]);

        //Setting second test position
        testPosition[0] = photon.time() + (stepSize/2.)*kTarray[0];
        testPosition[1] = photon.radius() + (stepSize/2.)*kRarray[0];
        testPosition[2] = photon.theta() + (stepSize/2.)*kThArray[0];
        testPosition[3] = photon.phi() + (stepSize/2.)*kPhiArray[0];

        //Second set of k's for rk4 integration
        kTarray[1] = photon.tdot(metric, testPosition);
        kRsqArray[1] = photon.rdotsq(metric, testPosition);
        kThSqArray[1] = photon.thdotsq(metric, testPosition);
        kPhiArray[1] = photon.phidot(metric, testPosition);

        //Checking switches for r^2 and theta^2
        checkSwitches(photon, kRsqArray[1], kThSqArray[1]);

        //Taking sqrt of r^2 and theta^2, setting sign to be consistent with switches
        kRarray[1] = photon.getRswitch()*sqrt(std::fabs(kRsqArray[1]));
        kThArray[1] = photon.getThetaSwitch()*sqrt(std::fabs(kThSqArray[1]));

        //Setting third test position
        testPosition[0] = photon.time() + (stepSize/2.)*kTarray[1];
        testPosition[1] = photon.radius() + (stepSize/2.)*kRarray[1];
        testPosition[2] = photon.theta() + (stepSize/2.)*kThArray[1];
        testPosition[3] = photon.phi() + (stepSize/2.)*kPhiArray[1];

        //Third set of k's for rk4 integration
        kTarray[2] = photon.tdot(metric, testPosition);
        kRsqArray[2] = photon.rdotsq(metric, testPosition);
        kThSqArray[2] = photon.thdotsq(metric, testPosition);
        kPhiArray[2] = photon.phidot(metric, testPosition);

        //Checking switches for r^2 and theta^2
        checkSwitches(photon, kRsqArray[2], kThSqArray[2]);

        //Taking sqrt of r^2 and theta^2, setting sign to be consistent with switches
        kRarray[2] = photon.getRswitch()*sqrt(std::fabs(kRsqArray[2]));
        kThArray[2] = photon.getThetaSwitch()*sqrt(std::fabs(kThSqArray[2]));

        //Setting fourth test position
        testPosition[0] = photon.time() + stepSize*kTarray[2];
        testPosition[1] = photon.radius() + stepSize*kRarray[2];
        testPosition[2] = photon.theta()+ stepSize*kThArray[2];
        testPosition[3] = photon.phi() + stepSize*kPhiArray[2];

        //Fourth set of k's for rk4 integration
        kTarray[3] = photon.tdot(metric, testPosition);
        kRsqArray[3] = photon.rdotsq(metric, testPosition);
        kThSqArray[3] = photon.thdotsq(metric, testPosition);
        kPhiArray[3] = photon.phidot(metric, testPosition);

        //Checking switches for r^2 and theta^2
        checkSwitches(photon, kRsqArray[3], kThSqArray[3]);

        //Taking sqrt of r^2 and theta^2, setting sign to be consistent with switches
        kRarray[3] = photon.getRswitch()*sqrt(std::fabs(kRsqArray[3]));
        kThArray[3] = photon.getThetaSwitch()*sqrt(std::fabs(kThSqArray[3]));

        //Calculating the amount the that each spacetime component will advance by for each total step (RK4)
        tStep = (stepSize/6.)*(kTarray[0]+(2.*kTarray[1])+(2.*kTarray[2])+kTarray[3]);
        rStep = (stepSize/6.)*(kRarray[0]+(2.*kRarray[1])+(2.*kRarray[2])+kRarray[3]);
        thStep = (stepSize/6.)*(kThArray[0]+(2.*kThArray[1])+(2.*kThArray[2])+kThArray[3]);
        phiStep = (stepSize/6.)*(kPhiArray[0]+(2.*kPhiArray[1])+(2.*kPhiArray[2])+kPhiArray[3]);

        //Updating the photon's position vector
        newPosition[0] = photon.time() + tStep;
        newPosition[1] = photon.radius() + rStep;
        newPosition[2] = photon.theta() + thStep;
        newPosition[3] = photon.phi() + phiStep;
		photon.updatePosition(newPosition);

        //Updating the photon's momentum vector
        newMomentum[0] = tStep/stepSize;
        newMomentum[1] = rStep/stepSize;
        newMomentum[2] = thStep/stepSize;
        newMomentum[3] = phiStep/stepSize;
        photon.updateMomentum(newMomentum);

        //Incrementing the integration counter
        i++;
	}
}
