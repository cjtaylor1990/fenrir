#include <cmath>

double eTT(double r, double phi, double theta, double time, double rotOmega){
	double bottomTerm1,bottomTerm2,bottomTerm3,eTTout;
	bottomTerm1 = gTT(r,phi,theta,time);
	bottomTerm2 = (gTPh(r,phi,theta,time) + gPhT(r,phi,theta,time))*rotOmega;
	bottomTerm3 = gPhPh(r,phi,theta,time)*rotOmega*rotOmega;
	eTTout = -1.*(bottomTerm1+bottomTerm2+bottomTerm3);
	eTTout = 1./sqrt(eTTout);
	return eTTout;
}

double eTPh(double r, double phi, double theta, double time, double rotOmega){
	return rotOmega*eTT(r,phi,theta,time,rotOmega);
}

double eRR(double r, double phi, double theta, double time, double rotOmega){
	return 1./sqrt(gRR(r,phi,theta,time));
}

double eThTh(double r, double phi, double theta, double time, double rotOmega){
	return 1./sqrt(gThTh(r,phi,theta,time));
}

double cFunct(double r, double phi, double theta, double time, double rotOmega){
	double topTerm, bottomTerm;
	topTerm = -1.*(gPhT(r,phi,theta,time) + gPhPh(r,phi,theta,time)*rotOmega);
	bottomTerm = (gTT(r,phi,theta,time) + gTPh(r,phi,theta,time)*rotOmega);
	return topTerm/bottomTerm;
}

double ePhPh(double r, double phi, double theta, double time, double rotOmega){
	double cFunctVal = cFunct(r,phi,theta,time,rotOmega);
	double bottomTerm1,bottomTerm2,bottomTerm3,ePhPhOut;
	bottomTerm1 = gPhPh(r,phi,theta,time);
	bottomTerm2 = (gTPh(r,phi,theta,time) + gPhT(r,phi,theta,time))*cFunctVal;
	bottomTerm3 = gTT(r,phi,theta,time)*cFunctVal*cFunctVal;
	ePhPhOut = bottomTerm1 + bottomTerm2 + bottomTerm3;
	return 1./sqrt(ePhPhOut);
}

double ePhT(double r, double phi, double theta, double time, double rotOmega){
	return cFunct(r,phi,theta,time,rotOmega)*ePhPh(r,phi,theta,time,rotOmega);
}
