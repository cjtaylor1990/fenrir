#include <cmath>

double sigma(double r, double phi, double theta, double time){
    double cosTerm = std::cos(theta);
    double sigmaOut = (r*r)+(a*a*cosTerm*cosTerm);
	return sigmaOut;
	}
	
double delta(double r, double phi, double theta, double time){
    double deltaOut = (r*r)+(-2.*r)+(a*a);
	return deltaOut;
	}

double aFunct(double r, double phi, double theta, double time){
    double sinTerm = std::sin(theta);
    double deltaTerm = delta(r,phi,theta,time)*a*a*sinTerm*sinTerm;
    double aFunctOut = (((r*r)+(a*a))*(((r*r)+(a*a))))-deltaTerm;
	return aFunctOut;
	}
	
double epsFunct(double r, double phi, double theta, double time, double imgAlpha, double imgBeta){
    double sigmaTerm = sqrt(sigma(r,phi,theta,time));
    double deltaTerm = sqrt(delta(r,phi,theta,time));
    double aFunctTerm = sqrt(aFunct(r,phi,theta,time));
    double epsFunctTerm = ((sigmaTerm*deltaTerm)+((2.*a*r/sigmaTerm)*std::sin(theta)*std::sin(imgAlpha)*std::sin(imgBeta)))/aFunctTerm;
	return epsFunctTerm;
	}

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