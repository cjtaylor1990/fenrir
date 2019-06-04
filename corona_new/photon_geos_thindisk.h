#include <cmath>

double tdot(double r, double phi, double theta, double time){
    double sinTerm = std::sin(theta);
    double energyTerm = ((sigma(r,phi,theta,time)*((r*r)+(a*a)))+(2.*r*a*a*sinTerm*sinTerm))*energy;
	double angmomTerm = -2.*a*r*angmom;
	double bottomTerm1 = (sigma(r,phi,theta,time) - (2.*r))*((r*r)+(a*a));
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm;
	return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
	}

double phidot(double r, double phi, double theta, double time){
    double sinTerm = std::sin(theta);
    double energyTerm = 2.*a*r*sinTerm*sinTerm*energy;
	double angmomTerm = (sigma(r,phi,theta,time) - (2.*r))*angmom;
    double bottomTerm1 = (sigma(r,phi,theta,time) - (2.*r))*((r*r)+(a*a))*sinTerm*sinTerm;
    double bottomTerm2 = 2.*r*a*a*sinTerm*sinTerm*sinTerm*sinTerm;
	return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
	}

double thdotsq(double r, double phi, double theta, double time){
    double cosTerm = std::cos(theta);
    double cotTerm = 1./std::tan(theta);
    double aTerm = a*a*cosTerm*cosTerm*energy;
    double angmomTerm = -1.*angmom*angmom*cotTerm*cotTerm;
    double topTerm = carter+aTerm+angmomTerm;
    double bottomTerm = sigma(r,phi,theta,time)*sigma(r,phi,theta,time);
	return topTerm/bottomTerm;
	}
	
double rdotsq(double r, double phi, double theta, double time){
	double energyTerm = energy*tdot(r, phi, theta, time);
	double angmomTerm = -1.*angmom*phidot(r, phi, theta, time);
    double carterTerm = -1.*sigma(r,phi,theta,time)*thdotsq(r,phi,theta,time);
	double frontTerm = delta(r, phi, theta, time) / sigma(r, phi, theta, time);
	return (frontTerm*(energyTerm+angmomTerm+carterTerm));
	}