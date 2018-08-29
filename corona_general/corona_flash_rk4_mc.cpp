#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "ic1_rk4_corona_mc.h"
#include "metric_kerr.h"
#include "global_variables.h"
#include "photon_geos_thindisk.h"
#include "matter_geos_thindisk.h"
#include "propagate_rk4_thindisk.h"
#include "flash_angmom_carter.h"
#include "corona_basis.h"
#include "photmom_rest_iso.h"
#include "photmom.h"
#include "lnrf_basis.h"
#include "dblRandGen.h"

//main function
int main(int argc, char* argv[]){
	//Seeding the random number generator
	srand(seed);
	
    //Opening file that will be the output
    std::ofstream myfile;  //Defining the output file to be written to.
    myfile.open (outFileName);  //Opening the output file.
    
    //Dummy variable
    int j;
    
    //Finding initial positions
    findPosition();
    
    //Finding rotational velocity Omega (dphi/dt)
    findOmega();
    
    //!!! Could do this calculation in a header file
    //Calculating the corona orthonormal tetrad components
    double eTTval,eTPhVal,eRRval,eThThVal,ePhTval,ePhPhVal;
    eTTval = eTT(initRadius,initPhi,initTheta,initTime,rotOmega);
    eTPhVal = eTPh(initRadius,initPhi,initTheta,initTime,rotOmega);
    eRRval = eRR(initRadius,initPhi,initTheta,initTime,rotOmega);
    eThThVal = eThTh(initRadius,initPhi,initTheta,initTime,rotOmega);
    ePhTval = ePhT(initRadius,initPhi,initTheta,initTime,rotOmega);
    ePhPhVal = ePhPh(initRadius,initPhi,initTheta,initTime,rotOmega);
    
    ///!!! Could do this calculation in header file
    //Constructing the corona tetrad vectors
    double eTvec[4],eRvec[4],eThVec[4],ePhVec[4];
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
    
    //Calculating value of metric components at corona position
    double gTTval,gTPhVal,gRRval,gThThVal,gPhTval,gPhPhVal;
    gTTval = gTT(initRadius,initPhi,initTheta,initTime);
    gTPhVal = gTPh(initRadius,initPhi,initTheta,initTime);
    gRRval = gRR(initRadius,initPhi,initTheta,initTime);
    gThThVal = gThTh(initRadius,initPhi,initTheta,initTime);
    gPhTval = gPhT(initRadius,initPhi,initTheta,initTime);
    gPhPhVal = gPhPh(initRadius,initPhi,initTheta,initTime);
    
    //Calculating trig values at position of corona (used for carter calculation)
    double corCos,corSin,corCot;
    corCos = std::cos(initTheta);
    corSin = std::sin(initTheta);
    corCot = corCos/corSin;
    
    j = 0;
    while (j < n){
      //Generating random angles: alpha (verticle), beta (horizontal)
      imgAlpha = randAlpha();  //Calculating the image verticle angle (alpha)
      imgBeta = randBeta();  //Calculating the image horizontal angle (beta)
      
      //!!! Could modularize
      //Calculating corona rest frame photon momentum vector
      double restPvec[4];
      restPvec[0] = -1./((gTTval*eTvec[0] + gTPhVal*eTvec[3]) + ((gTTval*ePhVec[0] + gTPhVal*ePhVec[3])*std::sin(imgAlpha)*std::sin(imgBeta)));//restPt();
      restPvec[1] = restPvec[0]*restPr(imgAlpha,imgBeta);
      restPvec[2] = restPvec[0]*restPth(imgAlpha,imgBeta);
      restPvec[3] = restPvec[0]*restPph(imgAlpha,imgBeta);
      
      //!!! Could modularize (actually, probably combine with above rest-frame calculation to just clean up code)
      //Calculating initial B-L photon momentum vector
      double pVec[4];
      pVec[0] = pT(restPvec,eTvec,eRvec,eThVec,ePhVec);
      pVec[1] = pR(restPvec,eTvec,eRvec,eThVec,ePhVec);
      pVec[2] = pTh(restPvec,eTvec,eRvec,eThVec,ePhVec);
      pVec[3] = pPh(restPvec,eTvec,eRvec,eThVec,ePhVec);
      
      //!!! Could modularize
      //Calculating conserved quantities from B-L momentum vector
      energy = -1.*((gTTval*pVec[0]) + (gTPhVal*pVec[3]));
      //angmom = photonAngmom(initRadius,initPhi,initTheta,initTime,imgAlpha,imgBeta);
      //angmom = restPvec[0]*std::sin(imgAlpha)*std::sin(imgBeta)*corPsi;
      angmom = (gPhTval*pVec[0]) + (gPhPhVal*pVec[3]);
      carter = ((gThThVal*pVec[2])*(gThThVal*pVec[2])) - (corCos*corCos*a*a*energy*energy) + (angmom*angmom*corCot*corCot);
      
      hitDiskSwitch = 0;  //Setting the hitDiskSwitch equal to 0.  Will change to 1 if the photon reaches disk.

      //Determining the sign of rdot based upon the verticle angle (alpha)
      if (imgAlpha < M_PI/2.){
        rSqrtSwitch = 1.;
      }else{
        rSqrtSwitch = -1.;
      }

      //Determining the sign of thetadot based upon the horizontal angle (beta)
      if ((imgBeta < M_PI/2.) || (1.5*M_PI < imgBeta)){
        thSqrtSwitch = 1.;
      }else{
        thSqrtSwitch = -1.;
      }

      //Giving the photon's 4-vector its initial conditions
      posVec[0] = initTime;
      posVec[1] = initRadius;
      posVec[2] = initTheta;
      posVec[3] = initPhi;

      //Propagate the photon to the disk
      propagate(posVec,momVec,dStep,tolerance,maxStep,rLimitLow,rLimitHigh,rEvent,scaleHeightValue,rProjected);

	  //!!! Modularize
      //Calculating the projected radius and the scale height of the disk
      rProjected = posVec[1]*std::sin(posVec[2]);
      scaleHeightValue = heightFrontTerm*(1 - sqrt(rIsco/rProjected));
      
      //!!! Modularize
      //Calculating the one-form of the photon's momentum 4-vector
      momOneForm[0] = (gTT(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[0])+(gTPh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[3]);
      momOneForm[1] = (gRR(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[1]);
      momOneForm[2] = (gThTh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[2]);
      momOneForm[3] = (gPhT(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[0])+(gPhPh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[3]);
      
      //!!! Modularize
      //Calculating the disk's velocity 4-vector
      diskVelVec[0] = diskTdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[1] = diskRdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[2] = diskThDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[3] = diskPhiDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);

      //Calculating the final energy of the photon (-1 x dot product of photon one-form momentum and the disk 4-velocity)
      finalEnergy = (momOneForm[0]*diskVelVec[0]) + (momOneForm[1]*diskVelVec[1]) + (momOneForm[2]*diskVelVec[2]) + (momOneForm[3]*diskVelVec[3]);
      finalEnergy = -1.*finalEnergy;

      //!!! Modularize. Also, like could condense this quite a bit
      //Calculating the gamma (Lorentz factor) of the disk element as seen from LNRF.  This is done by multiplying the 4-vector by the basis one-forms (i.e. the dot product)
      lorentzVelVec[0] = diskVelVec[0]*eNu(posVec[1],posVec[3],posVec[2],posVec[0]);
      lorentzVelVec[1] = diskVelVec[1]*eMu1(posVec[1],posVec[3],posVec[2],posVec[0]);
      lorentzVelVec[2] = diskVelVec[2]*eMu2(posVec[1],posVec[3],posVec[2],posVec[0]);
      lorentzVelVec[3] = (-1.*lnrfOmega(posVec[1],posVec[3],posVec[2],posVec[0])*ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[0])+(ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[3]);
      lorentzVelSq = ((lorentzVelVec[1]*lorentzVelVec[1]) + (lorentzVelVec[2]*lorentzVelVec[2]) + (lorentzVelVec[3]*lorentzVelVec[3]))/(lorentzVelVec[0]*lorentzVelVec[0]);
      lorentz = 1./sqrt(1.-lorentzVelSq);//diskVelVec[0]*sqrt(sigma(posVec[1],posVec[3],posVec[2],posVec[0])*delta(posVec[1],posVec[3],posVec[2],posVec[0])/aFunct(posVec[1],posVec[3],posVec[2],posVec[0]));

      //Printing to output file
      //myfile << imgAlpha << " " << imgBeta << " " << gammaVelVec[0] << " " << gamma << " " << hitDiskSwitch << "\n";
      myfile << imgAlpha << " " << imgBeta << " " << (finalEnergy/energy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << " " << lorentz << " " << hitDiskSwitch << "\n";
      //myfile << cFunct(initRadius,initPhi,initTheta,initTime,rotOmega) << " " << eTTval << " " << eTPhVal << " " << ePhTval << " " << ePhPhVal <<"\n";
      //myfile << imgAlpha << " " << imgBeta << " " << energy << " " << angmom << " " << carter << " " << pVec[2] << " " << restPvec[2] << " " << eThThVal << " " << gThThVal << "\n";
      j++;
    }
    myfile.close();  //Closing the output file.
    return 0;  //returning integer value of 0 if executed to completion

}  //end of main function
