#include <cmath>

double diskEnergy(double rProjected){
    double energyTop = pow(rProjected,2.) - (2.*rProjected) + a*pow(rProjected,0.5);
    double energyBottom = pow(rProjected,2.) - (3.*rProjected) + (2.*a*pow(rProjected,0.5));
    return energyTop/(rProjected*pow(energyBottom,0.5));
}

double diskAngmom(double rProjected){
    double energyTop = pow(rProjected,0.5)*((pow(rProjected,2.))-(2.*a*pow(rProjected,0.5))+(pow(a,2.)));
    double energyBottom = pow(rProjected,2.) - (3.*rProjected) + (2.*a*pow(rProjected,0.5));
    return energyTop/(rProjected*pow(energyBottom,0.5));
}

double planeTdot(double r, double phi, double theta, double time, double rProjected){
    double energyDisk = diskEnergy(rProjected);
    double angmomDisk = diskAngmom(rProjected);
    double sinTerm = std::sin(theta);
    double diskSigma = sigma(rProjected,phi,M_PI/2.,time);
    double energyTerm = ((diskSigma*((rProjected*rProjected)+(a*a)))+(2.*rProjected*a*a*sinTerm*sinTerm))*energyDisk;
    double angmomTerm = -2.*a*rProjected*angmomDisk;
    double bottomTerm1 = (diskSigma - (2.*rProjected))*((rProjected*rProjected)+(a*a));
    double bottomTerm2 = 2.*rProjected*a*a*sinTerm*sinTerm;
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double planePhiDot(double r, double phi, double theta, double time, double rProjected){
    double energyDisk = diskEnergy(rProjected);
    double angmomDisk = diskAngmom(rProjected);
    double sinTerm = std::sin(theta);
    double diskSigma = sigma(rProjected,phi,M_PI/2.,time);
    double energyTerm = 2.*a*rProjected*sinTerm*sinTerm*energyDisk;
    double angmomTerm = (diskSigma - (2.*rProjected))*angmomDisk;
    double bottomTerm1 = (diskSigma - (2.*rProjected))*((rProjected*rProjected)+(a*a))*sinTerm*sinTerm;
    double bottomTerm2 = 2.*rProjected*a*a*sinTerm*sinTerm*sinTerm*sinTerm;
    return (energyTerm+angmomTerm)/(bottomTerm1+bottomTerm2);
}

double diskPhiVel(double r, double phi, double theta, double time, double scaleHeightValue, double rProjected){
    double planePhiDotValue = planePhiDot(r,phi,theta,time,rProjected);
    double planeTdotValue = planeTdot(r,phi,theta,time,rProjected);
    double augTerm = 1.;// - ((scaleHeightValue/rProjected)*(scaleHeightValue/rProjected));
    return (planePhiDotValue/planeTdotValue)*augTerm;
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
