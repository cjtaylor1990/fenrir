#include <cmath>
#include "metric.hpp"

Metric::Metric(double spin) {
    a = spin; 
}

double Metric::getSpin() {
    return a;
}

double Metric::sigma(double r, double phi, double theta, double time){
    double cosTerm = std::cos(theta);
	return (r*r)+(a*a*cosTerm*cosTerm);
}

double Metric::delta(double r, double phi, double theta, double time){
	return (r*r)+(-2.*r)+(a*a);
}

double Metric::gTT(double r, double phi, double theta, double time){
	return -1.*(1.-(2.*r/sigma(r,phi,theta,time)));
}

double Metric::gTPh(double r, double phi, double theta, double time){
    return (-2.*a*r*pow(std::sin(theta),2.))/sigma(r,phi,theta,time);
}

double Metric::gPhT(double r, double phi, double theta, double time){
    return (-2.*a*r*pow(std::sin(theta),2.))/sigma(r,phi,theta,time);
}

double Metric::gRR(double r, double phi, double theta, double time){
    return sigma(r,phi,theta,time)/delta(r,phi,theta,time);
}

double Metric::gThTh(double r, double phi, double theta, double time){
    return sigma(r,phi,theta,time);
}

double Metric::gPhPh(double r, double phi, double theta, double time){
    return (pow(r,2.)+pow(a,2.)+((2.*r*pow(a*std::sin(theta),2.))/sigma(r,phi,theta,time)))*pow(std::sin(theta),2.);
}
