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

//Functions to calculate orthonormal tetrad that is co-moving with corona
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


//Functions for initial momentum components in co-moving coronal frame
double restE(void){
	double restEout;
	restEout = 1.;
	return restEout;
}

double restPt(void){
	double restPtOut;
	restPtOut = restE();
	return restPtOut;
}

double restPr(double alpha, double beta){
	double restPrOut;
	restPrOut = restE()*std::cos(alpha);
	return restPrOut;
}

double restPth(double alpha, double beta){
	double restPthOut;
	restPthOut = restE()*std::sin(alpha)*std::cos(beta);
	return restPthOut;
}

double restPph(double alpha, double beta){
	double restPphOut;
	restPphOut = restE()*std::sin(alpha)*std::sin(beta);
	return restPphOut;
}

//Functions for initial momentum components in B-L coordinates
double pT(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[0]*eTvec[0]) + (restMomVec[3]*ePhVec[0]);
}
	
double pR(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[1]*eRvec[1]);
}

double pTh(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[2]*eThVec[2]);
}

double pPh(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[0]*eTvec[3]) + (restMomVec[3]*ePhVec[3]);
}

//Function to calculate the corona tetrad vector components
void findTetrad(){//double eTvec[4],double eRvec[4], double eThVec[4], double ePhVec[4]){
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

//Function to calculate the metric components at the corona position
void findComponents(){//double gTTval, double gTPhVal, double gRRval, double gThThVal, double gPhTval, double gPhPhVal){
    gTTval = gTT(initRadius,initPhi,initTheta,initTime);
    gTPhVal = gTPh(initRadius,initPhi,initTheta,initTime);
    gRRval = gRR(initRadius,initPhi,initTheta,initTime);
    gThThVal = gThTh(initRadius,initPhi,initTheta,initTime);
    gPhTval = gPhT(initRadius,initPhi,initTheta,initTime);
    gPhPhVal = gPhPh(initRadius,initPhi,initTheta,initTime);
}

//Function to calculate the initial photon momentum vector in B-L coordinates
void findMomentum(){//double momVec[4]){
	double restPvec[4];
    restPvec[0] = -1./((gTTval*eTvec[0] + gTPhVal*eTvec[3]) + ((gTTval*ePhVec[0] + gTPhVal*ePhVec[3])*std::sin(imgAlpha)*std::sin(imgBeta)));//restPt();
    restPvec[1] = restPvec[0]*restPr(imgAlpha,imgBeta);
    restPvec[2] = restPvec[0]*restPth(imgAlpha,imgBeta);
    restPvec[3] = restPvec[0]*restPph(imgAlpha,imgBeta);
      
    //Calculating initial B-L photon momentum vector
    momVec[0] = pT(restPvec,eTvec,eRvec,eThVec,ePhVec);
    momVec[1] = pR(restPvec,eTvec,eRvec,eThVec,ePhVec);
    momVec[2] = pTh(restPvec,eTvec,eRvec,eThVec,ePhVec);
    momVec[3] = pPh(restPvec,eTvec,eRvec,eThVec,ePhVec);
}

//Function to calculate the conserved quantities of the photons
void findConserved(){//double energy, double angmom, double carter){
	energy = -1.*((gTTval*momVec[0]) + (gTPhVal*momVec[3]));
    angmom = (gPhTval*momVec[0]) + (gPhPhVal*momVec[3]);
    carter = ((gThThVal*momVec[2])*(gThThVal*momVec[2])) - (corCos*corCos*a*a*energy*energy) + (angmom*angmom*corCot*corCot);
}
      