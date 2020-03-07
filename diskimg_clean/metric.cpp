#include <cmath>
#include "metric.hpp"

Metric::Metric(double spin) {
    a = spin; 
	rEvent = 1. + sqrt(1.-(a*a));
}

double Metric::spin() {
    return a;
}

double Metric::eventHorizon() {
	return rEvent;
}

double Metric::sigma(double position[4]){
    double cosTerm = std::cos(position[2]);

	return (position[1]*position[1])+(a*a*cosTerm*cosTerm);
}

double Metric::delta(double position[4]){
	return (position[1]*position[1])+(-2.*position[1])+(a*a);
}

double Metric::gTT(double position[4]){
	return -1.*(1.-(2.*position[1]/sigma(position)));
}

double Metric::gTPh(double position[4]){
    return (-2.*a*position[1]*pow(std::sin(position[2]),2.))/sigma(position);
}

double Metric::gPhT(double position[4]){
    return (-2.*a*position[1]*pow(std::sin(position[2]),2.))/sigma(position);
}

double Metric::gRR(double position[4]){
    return sigma(position)/delta(position);
}

double Metric::gThTh(double position[4]){
    return sigma(position);
}

double Metric::gPhPh(double position[4]){
    return (pow(position[1],2.)+pow(a,2.)+((2.*position[1]*pow(a*std::sin(position[2]),2.))/sigma(position)))*pow(std::sin(position[2]),2.);
}

void Metric::vectorToOneForm(double position[4], double vector[4], double oneForm[4]){

    //Calculating Kerr components
    double gTTval = gTT(position);
    double gPhTval = gPhT(position);
    double gTPhVal = gPhTval;
    double gRRval = gRR(position);
    double gThThVal = gThTh(position);
    double gPhPhVal = gPhPh(position);

    //Performing conversion
    oneForm[0] = gTTval*vector[0] + gTPhVal*vector[3];
    oneForm[1] = gRRval*vector[1];
    oneForm[2] = gThThVal*vector[2];
    oneForm[3] = gPhTval*vector[0] + gPhPhVal*vector[3];

}
