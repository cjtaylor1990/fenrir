#include <cmath>

//Functions used in calculating alpha and beta
double randAlpha(){
	double randVal = (double)rand()/RAND_MAX;
	return std::acos(-1. + (2.*randVal));
}
double randBeta(){
	double randVal = (double)rand()/RAND_MAX;
	return 2.*M_PI*randVal;
}

//Function to find the initial position of the corona in B-L coordinates
void findPosition(){
	if (coronaType == "lp"){
		initTime = 0.;
		initRadius = height;
		initTheta = (M_PI/180.)*0.01;
		initPhi = 0.;
	}else{
		if (isRisco == true){
			initCylRadius = initCylRadius*rIsco;
		}
		double thicknessAtCorona = heightFrontTerm*(1. - sqrt(rIsco/initCylRadius));
		initTime = 0.;
		initTheta = std::atan(initCylRadius/(thicknessAtCorona+heightAboveDisk));
		initRadius = initCylRadius/std::sin(initTheta);
		initPhi = coronaPhi;
	}
}

//Function to calculate the corona rotational velocity (dphi/dt = rotOmega)
void findOmega(){
	if ((coronaType == "lp") or ((coronaType == "offaxis") and (rotOmegaType == "lnrf"))){
		rotOmega = 2.*a*initRadius/aFunct(initRadius, initPhi, initTheta, initTime);
	}else if ((coronaType == "offaxis") and (rotOmegaType == "kep")){
		rotOmega = 1./(pow(initCylRadius,1.5) + a);
	}else{
		rotOmega = rotOmegaInput;
	}
}

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

//Function to calculate the corona tetrad vector components
void findTetrad(){
	double eTTval,eTPhVal,eRRval,eThThVal,ePhTval,ePhPhVal;
	
	eTTval = eTT(initRadius,initPhi,initTheta,initTime,rotOmega);
	eTPhVal = eTPh(initRadius,initPhi,initTheta,initTime,rotOmega);
	eRRval = eRR(initRadius,initPhi,initTheta,initTime,rotOmega);
	eThThVal = eThTh(initRadius,initPhi,initTheta,initTime,rotOmega);
	ePhTval = ePhT(initRadius,initPhi,initTheta,initTime,rotOmega);
	ePhPhVal = ePhPh(initRadius,initPhi,initTheta,initTime,rotOmega);

    //Time basis vector
    eTvec[0] = eTTval;
    eTvec[1] = 0.;
    eTvec[2] = 0.;
    eTvec[3] = eTPhVal;
    //Radial basis vector
    eRvec[0] = 0.;
    eRvec[1] = eRRval;
    eRvec[2] = 0.;
    eRvec[3] = 0.;
    //Theta basis vector
    eThVec[0] = 0.;
    eThVec[1] = 0.;
    eThVec[2] = eThThVal;
    eThVec[3] = 0.;
    //Phi basis vector
    ePhVec[0] = ePhTval;
    ePhVec[1] = 0.;
    ePhVec[2] = 0.;
    ePhVec[3] = ePhPhVal;
    
}

