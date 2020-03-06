#include <cmath>
#include "photon.hpp"
#include "metric.hpp"

Photon::Photon(double newR, double newPhi, double newTheta, double newTime, double newEnergy, double newAngMom, double newCarter){
    r = newR;
    phi = newPhi;
    theta = newTheta;
    time = newTime;
    energy = newEnergy;
    angMom = newAngMom;
    carter = newCarter;
}

double Photon::getRadius() {
    return r;
}

double Photon::getPhi() {
    return phi;
}

double Photon::getTheta() {
    return theta;
}

double Photon::getTime() {
    return time;
}

void Photon::updatePosition(double newR, double newPhi, double newTheta, double newTime) {
    r = newR;
    phi = newPhi;
    theta = newTheta;
    time = newTime;
}

double Photon::tdot(Metric metric){
    double a = metric.getSpin();
    double sinTerm = std::sin(theta);

    double energyTerm = ((metric.sigma(r,phi,theta,time)*((r*r)+(a*a)))+(2.*r*a*a*sinTerm*sinTerm))*energy;
	double angmomTerm = -2.*a*r*Photon::angMom;
	double bottomTerm1 = (metric.sigma(r,phi,theta,time) - (2.*r))*((r*r)+(a*a));
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm;
	
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double Photon::phidot(Metric metric){
    double a = metric.getSpin();
    double sinTerm = std::sin(theta);
    
    double energyTerm = 2.*a*r*sinTerm*sinTerm*energy;
	double angmomTerm = (metric.sigma(r,phi,theta,time) - (2.*r))*angMom;
    double bottomTerm1 = (metric.sigma(r,phi,theta,time) - (2.*r))*((r*r)+(a*a))*sinTerm*sinTerm;
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm*sinTerm*sinTerm;
	
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double Photon::thdotsq(Metric metric){
    double a = metric.getSpin();
    double cosTerm = std::cos(theta);
    double cotTerm = 1./std::tan(theta);
    double sigma = metric.sigma(r, phi, theta, time);
    
    double aTerm = a*a*cosTerm*cosTerm*energy;
    double angMomTerm = -1.*angMom*angMom*cotTerm*cotTerm;
    double topTerm = carter+aTerm+angMomTerm;
    double bottomTerm = sigma*sigma;
	
    return topTerm/bottomTerm;
}

double Photon::rdotsq(Metric metric){
	double energyTerm = energy*tdot(metric);
	double angmomTerm = -1.*angMom*phidot(metric);
    double carterTerm = -1.*metric.sigma(r,phi,theta,time)*thdotsq(metric);
	double frontTerm = metric.delta(r, phi, theta, time) / metric.sigma(r, phi, theta, time);
	
    return (frontTerm*(energyTerm+angmomTerm+carterTerm));
}
