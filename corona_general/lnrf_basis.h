#include <cmath>

double eNu(double r, double phi, double theta, double time){
	double eNuOut = (sigma(r,phi,theta,time)*delta(r,phi,theta,time))/aFunct(r,phi,theta,time);
	eNuOut = sqrt(eNuOut);
	return eNuOut;
}

double ePsi(double r, double phi, double theta, double time){
	double sinVal = std::sin(theta);
	double ePsiOut = aFunct(r,phi,theta,time)/sigma(r,phi,theta,time);
	ePsiOut = sqrt((sinVal*sinVal)*ePsiOut);
	return ePsiOut;
}

double eMu1(double r, double phi, double theta, double time){
	return sqrt(sigma(r,phi,theta,time)/delta(r,phi,theta,time));
}

double eMu2(double r, double phi, double theta, double time){
	return sqrt(sigma(r,phi,theta,time));
}

double lnrfOmega(double r, double phi, double theta, double time){
	return (2.*a*r)/aFunct(r,phi,theta,time);
}