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

void vecToOneForm(double posVec[4], double vec[4], double oneForm[4]){
    //Defining local location variables to use
    double localTime = posVec[0];
    double localRadius = posVec[1];
    double localTheta = posVec[2];
    double localPhi = posVec[3];

    //Calculating sin(theta)^2 value
    double sinVal = std::sin(localTheta);
    double sinTerm = sinVal*sinVal;

    //Calculating Kerr components
    double gTTval = gTT(localRadius,localPhi,localTheta,localTime);
    double gPhTval = gPhT(localRadius,localPhi,localTheta,localTime);
    double gTPhVal = gPhTval;
    double gRRval = gRR(localRadius,localPhi,localTheta,localTime);
    double gThThVal = gThTh(localRadius,localPhi,localTheta,localTime);
    double gPhPhVal = gPhPh(localRadius,localPhi,localTheta,localTime);

    //Performing conversion
    oneForm[0] = gTTval*vec[0] + gTPhVal*vec[3];
    oneForm[1] = gRRval*vec[1];
    oneForm[2] = gThThVal*vec[2];
    oneForm[3] = gPhTval*vec[0] + gPhPhVal*vec[3];

}
