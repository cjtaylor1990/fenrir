#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "ic1_rk4_corona_mc.h"
#include "photon_geos_thindisk.h"
#include "metric_kerr.h"
#include "matter_geos_thindisk.h"
#include "propagate_rk4_thindisk.h"
#include "flash_angmom_carter.h"
#include "lnrf_basis.h"
#include "dblRandGen.h"

//main function
int main(int argc, char* argv[]){
    //Opening file that will be the output
    std::ofstream myfile;  //Defining the output file to be written to.
    myfile.open (outFileName);  //Opening the output file.

    //Calculating radius of event horizon
    double rEvent = 1. - pow(1.-(a*a),0.5);

    //Defining two distances of interest to be calculated at end of integration
    double scaleHeightValue;  //Defining the variable that will hold the scale height where the photon ends up
    double rProjected;  //Defining the variable that will hold the radial distance from the final placement of the photon to the black hole, projected onto the plane

    //Looping over sky image (Nx2N)
    int j,k;  //Defining integration dummy variables. J loops over alpha (verticle angle) while K loops over the horizontal angle (beta)
    double imgAlpha,imgBeta;  //Defining the angle variables
    srand(time(NULL));
    //double stepAlpha = 2./n;  //Defining the d(cos(alpha)) variable.  This is to break up alpha into equal spacing in cos(alpha)
    //double stepBeta = (2.*M_PI)/n;  //Defining the d(beta) variable.  Breaks up beta into equal spaces.
    j = 0;
    while (j < n){
      imgAlpha = std::acos(dblRandAlpha());  //Calculating the image verticle angle (alpha)
      imgBeta = 2.*M_PI*dblRandBeta();  //Calculating the image horizontal angle (beta)
      angmom = photonAngmom(initRadius,initPhi,initTheta,initTime,imgAlpha,imgBeta);  //Calculating the photon phi angular momentum
      carter = photonCarter(initRadius,initPhi,initTheta,initTime,imgAlpha,imgBeta);  //Calculating the photon Carter constant
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

      //Calculating the projected radius and the scale height of the disk
      rProjected = posVec[1]*std::sin(posVec[2]);
      scaleHeightValue = heightFrontTerm*(1 - sqrt(rIsco/rProjected));

      //Calculating the one-form of the photon's momentum 4-vector
      double momOneForm[4];
      momOneForm[0] = (gTT(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[0])+(gTPh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[3]);
      momOneForm[1] = (gRR(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[1]);
      momOneForm[2] = (gThTh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[2]);
      momOneForm[3] = (gPhT(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[0])+(gPhPh(posVec[1],posVec[3],posVec[2],posVec[0])*momVec[3]);

      //Calculating the disk's velocity 4-vector
      double diskVelVec[4];
      diskVelVec[0] = diskTdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[1] = diskRdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[2] = diskThDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);
      diskVelVec[3] = diskPhiDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected);

      //Calculating the final energy of the photon (-1 x dot product of photon one-form momentum and the disk 4-velocity)
      double finalEnergy = (momOneForm[0]*diskVelVec[0]) + (momOneForm[1]*diskVelVec[1]) + (momOneForm[2]*diskVelVec[2]) + (momOneForm[3]*diskVelVec[3]);
      finalEnergy = -1.*finalEnergy;

      //Calculating the gamma (Lorentz factor) of the disk element as seen from LNRF.  This is done by multiplying the 4-vector by the basis one-forms (i.e. the dot product)
      double gamma,gammaVelSq; //Note that gammaVelSq is the 3-velocity squared and not 4-velocity
      double gammaVelVec[4];
      gammaVelVec[0] = diskVelVec[0]*eNu(posVec[1],posVec[3],posVec[2],posVec[0]);
      gammaVelVec[1] = diskVelVec[1]*eMu1(posVec[1],posVec[3],posVec[2],posVec[0]);
      gammaVelVec[2] = diskVelVec[2]*eMu2(posVec[1],posVec[3],posVec[2],posVec[0]);
      gammaVelVec[3] = (-1.*lnrfOmega(posVec[1],posVec[3],posVec[2],posVec[0])*ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[0])+(ePsi(posVec[1],posVec[3],posVec[2],posVec[0])*diskVelVec[3]);
      gammaVelSq = ((gammaVelVec[1]*gammaVelVec[1]) + (gammaVelVec[2]*gammaVelVec[2]) + (gammaVelVec[3]*gammaVelVec[3]))/(gammaVelVec[0]*gammaVelVec[0]);
      gamma = 1./sqrt(1.-gammaVelSq);//diskVelVec[0]*sqrt(sigma(posVec[1],posVec[3],posVec[2],posVec[0])*delta(posVec[1],posVec[3],posVec[2],posVec[0])/aFunct(posVec[1],posVec[3],posVec[2],posVec[0]));

      //Printing to output file
      //myfile << imgAlpha << " " << imgBeta << " " << gammaVelVec[0] << " " << gamma << " " << hitDiskSwitch << "\n";
      myfile << imgAlpha << " " << imgBeta << " " << (finalEnergy/energy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << " " << gamma << " " << hitDiskSwitch << "\n";
      //myfile << imgAlpha << " " << imgBeta << " " << finalEnergy << " " << posVec[1] << " " << angmom << " " << carter << "\n";
      j++;
    }
    myfile.close();  //Closing the output file.
    return 0;  //returning integer value of 0 if executed to completion

}  //end of main function
