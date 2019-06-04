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

//Disk velocity components
double diskPhiVel(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
    return 1./(sqrt(rProjected*rProjected*rProjected)+(a));
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

//Calculate the disk velocity at the position that the photon is intercepted
void diskVelocity(double posVec[4], double diskVelVec[4], double scaleHeightValue, double rProjected){
	diskVelVec[0] = diskTdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //time
    diskVelVec[1] = diskRdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //radius
    diskVelVec[2] = diskThDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //theta
    diskVelVec[3] = diskPhiDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //phi
    }

//LNRF basis components to calculate the Lorentz factor of the disk element in the frame of the LNRF at the same instantaneous position    
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

//Rotational velocity of the LNRF
double lnrfOmega(double r, double phi, double theta, double time){
	return (2.*a*r)/aFunct(r,phi,theta,time);
}

//Function to calculate the Lorentz factor of the disk relative to the LNRF that it is instantaneously co-spatial
double findLorentz(double posVec[4], double diskVelVec[4]){
	double lorentzVelVec[4];
	double lorentzVelSq;
	lorentzVelVec[0] = diskVelVec[0]*eNu(posVec[1],posVec[3],posVec[2],posVec[0]);
    lorentzVelVec[1] = diskVelVec[1]*eMu1(posVec[1],posVec[3],posVec[2],posVec[0]);
    lorentzVelVec[2] = diskVelVec[2]*eMu2(posVec[1],posVec[3],posVec[2],posVec[0]);
    lorentzVelVec[3] = (-1.*lnrfOmega(posVec[1],posVec[3],posVec[2],posVec[0])*ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[0])+(ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[3]);
    lorentzVelSq = ((lorentzVelVec[1]*lorentzVelVec[1]) + (lorentzVelVec[2]*lorentzVelVec[2]) + (lorentzVelVec[3]*lorentzVelVec[3]))/(lorentzVelVec[0]*lorentzVelVec[0]);
    return 1./sqrt(1.-lorentzVelSq);
}
    
	  