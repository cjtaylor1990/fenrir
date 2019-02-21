#include <cmath>

double gTT(double r, double phi, double theta, double time){
	return -1.*(1.-(2.*r/sigma(r,phi,theta,time)));
}

double gTPh(double r, double phi, double theta, double time){
    return (-2.*a*r*pow(std::sin(theta),2.))/sigma(r,phi,theta,time);
}

double gPhT(double r, double phi, double theta, double time){
    return (-2.*a*r*pow(std::sin(theta),2.))/sigma(r,phi,theta,time);
}

double gRR(double r, double phi, double theta, double time){
    return sigma(r,phi,theta,time)/delta(r,phi,theta,time);
}

double gThTh(double r, double phi, double theta, double time){
    return sigma(r,phi,theta,time);
}

double gPhPh(double r, double phi, double theta, double time){
    return (pow(r,2.)+pow(a,2.)+((2.*r*pow(a*std::sin(theta),2.))/sigma(r,phi,theta,time)))*pow(std::sin(theta),2.);
}