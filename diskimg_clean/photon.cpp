#include <cmath>
#include "photon.hpp"
#include "metric.hpp"

Photon::Photon(double newPosition[4], double newEnergy, double newAngMom, double newCarter, double newRswitch, double newThetaSwitch){
    position[0] = newPosition[0];
    position[1] = newPosition[1];
    position[2] = newPosition[2];
    position[3] = newPosition[3];
    energy = newEnergy;
    angMom = newAngMom;
    carter = newCarter;
	rSwitch = newRswitch;
	thetaSwitch =  newThetaSwitch;
}

double Photon::time() {
    return position[0];
}

double Photon::radius() {
    return position[1];
}

double Photon::theta() {
    return position[2];
}

double Photon::phi() {
    return position[3];
}

void Photon::updatePosition(double newPosition[4]) {
    position[0] = newPosition[0];
    position[1] = newPosition[1];
    position[2] = newPosition[2];
    position[3] = newPosition[3];
}

void Photon::updateMomentum(double newMomentum[4]) {
    momentum[0] = newMomentum[0];
	momentum[1] = newMomentum[1];
	momentum[2] = newMomentum[2];
	momentum[3] = newMomentum[3];
}

void Photon::updateRswitch(double newRswitch) {
	rSwitch = newRswitch;
}

void Photon::updateThetaSwitch(double newThetaSwitch) {
	thetaSwitch = newThetaSwitch;
}

double Photon::tdot(Metric metric, double testPosition[4]){
    double a = metric.spin();
    double r = testPosition[1];
    double theta = testPosition[2];
    double sinTerm = std::sin(theta);

    double energyTerm = ((metric.sigma(testPosition)*((r*r)+(a*a)))+(2.*r*a*a*sinTerm*sinTerm))*energy;
	double angmomTerm = -2.*a*r*Photon::angMom;
	double bottomTerm1 = (metric.sigma(testPosition) - (2.*r))*((r*r)+(a*a));
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm;
	
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double Photon::phidot(Metric metric, double testPosition[4]){
    double a = metric.spin();
    double r = testPosition[1];
    double theta = testPosition[2];
    double sinTerm = std::sin(theta);
    
    double energyTerm = 2.*a*r*sinTerm*sinTerm*energy;
	double angmomTerm = (metric.sigma(testPosition) - (2.*r))*angMom;
    double bottomTerm1 = (metric.sigma(testPosition) - (2.*r))*((r*r)+(a*a))*sinTerm*sinTerm;
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm*sinTerm*sinTerm;
	
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double Photon::thdotsq(Metric metric, double testPosition[4]){
    double a = metric.spin();
    double r = testPosition[1];
    double theta = testPosition[2];
    double cosTerm = std::cos(theta);
    double cotTerm = 1./std::tan(theta);
    
    double sigma = metric.sigma(testPosition);
    
    double aTerm = a*a*cosTerm*cosTerm*energy;
    double angMomTerm = -1.*angMom*angMom*cotTerm*cotTerm;
    double topTerm = carter+aTerm+angMomTerm;
    double bottomTerm = sigma*sigma;
	
    return topTerm/bottomTerm;
}

double Photon::rdotsq(Metric metric, double testPosition[4]){
	double energyTerm = energy*tdot(metric, testPosition);
	double angmomTerm = -1.*angMom*phidot(metric, testPosition);
    double carterTerm = -1.*metric.sigma(testPosition)*thdotsq(metric, testPosition);
	double frontTerm = metric.delta(testPosition) / metric.sigma(testPosition);
	
    return (frontTerm*(energyTerm+angmomTerm+carterTerm));
}
