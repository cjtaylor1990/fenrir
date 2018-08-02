#include <cmath>

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

double photonAngmom(double r, double phi, double theta, double time, double imgAlpha, double imgBeta){
    double bottomTerm = sqrt(sigma(r,phi,theta,time))*epsFunct(r,phi,theta,time,imgAlpha,imgBeta);
    double topTerm = sqrt(aFunct(r,phi,theta,time))*std::sin(theta)*std::sin(imgAlpha)*std::sin(imgBeta);
	return topTerm/bottomTerm;
	}

double photonCarter(double r, double phi, double theta, double time, double imgAlpha, double imgBeta){
    double angmomTerm = photonAngmom(r,phi,theta,time,imgAlpha,imgBeta);
    double sigmaTerm = sigma(r,phi,theta,time);
    double deltaTerm = delta(r,phi,theta,time);
    double epsFunctTerm = epsFunct(r,phi,theta,time,imgAlpha,imgBeta);
    double firstTerm = (((r*r)+(a*a)-(a*angmomTerm))*((r*r)+(a*a)-(a*angmomTerm)))/deltaTerm;
    double secondTerm = sigmaTerm*std::cos(imgAlpha)*std::cos(imgAlpha)/(epsFunctTerm*epsFunctTerm);
    double thirdTerm = (angmomTerm*angmomTerm) - (2.*a*angmomTerm) + (a*a);
    return (firstTerm - secondTerm - thirdTerm);
    }