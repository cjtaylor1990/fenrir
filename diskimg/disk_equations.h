#include <cmath>

//Function to calculate the disk scale height
double scaleHeightTheta(double radius, double theta){
  double scaleHTprojR = radius*std::sin(theta);
  if (scaleHTprojR < rIsco){
    return M_PI/2.;
  }
  else{
    return std::acos((heightFrontTerm*(1 - sqrt(rIsco/scaleHTprojR)))/radius);
  }
}

double diskPhiVel(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
    return 1./(sqrt(r*r*r)+(spinTermDisk*a));
}

double diskTdot(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
    double diskPhiVelValue = diskPhiVel(r,phi,theta,time,scaleHeightValue,rProjected);
    double gTTvalue = gTT(r,phi,theta,time);
    double gTPhValue = gTPh(r,phi,theta,time);
    double gPhPhValue = gPhPh(r,phi,theta,time);
    double diskTdotSqValue = -1.*(gTTvalue+(2.*gTPhValue*diskPhiVelValue)+(gPhPhValue*diskPhiVelValue*diskPhiVelValue));
    return sqrt(1./diskTdotSqValue);
}

double diskPhiDot(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
    double diskPhiVelValue = diskPhiVel(r,phi,theta,time,scaleHeightValue,rProjected);
    double diskTdotValue = diskTdot(r,phi,theta,time,scaleHeightValue,rProjected);
    return diskTdotValue*diskPhiVelValue;
}

double diskThDot(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
	return 0.;
	}

double diskRdot(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
	return 0.;
	}
