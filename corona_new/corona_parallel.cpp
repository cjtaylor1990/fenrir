#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "ic1_parallel.h"
#include "metric_kerr.h"
#include "global_variables.h"
#include "corona_functions_test.h"
#include "photon_geos_thindisk.h"
#include "disk_equations.h"
#include "propagate_rk4_thindisk.h"

//main function
int main(int argc, char* argv[]){
	//Seeding the random number generator
	srand(seed);
	
	//This divides the vertical dimension into parts, where each process does a certain number of rows
	int n, totProcs, procNum, photonPerProc;
	n = atoi(argv[1]); //total number of rows/columns for disk image (n x n photons)
	totProcs = atoi(argv[2]); //rows per process
	procNum = atoi(argv[3]); //index number of process (starts at 0)
	photonPerProc = n/totProcs; //number of photons per instance
	
    //Opening file that will be the output
    outFileName = argv[4]; //taking name as input from user
    std::ofstream myfile;  //Defining the output file to be written to.
    myfile.open (outFileName);  //Opening the output file.
    
    //Dummy variable
    int j;
    
    //Finding initial positions
    //findPosition(initTime,initRadius,initTheta,initPhi,initCylRadius); //in corona_functions.h
    //std::cout << initTime << " " << initRadius << " " << initTheta << " " << initPhi << "\n";
    //initTime = 0.;
	//initRadius = height;
	//initTheta = (M_PI/180.)*0.01;
	//initPhi = 0.;
	findPosition();
    
    //Finding rotational velocity Omega (dphi/dt)
    //findOmega(rotOmega); //in corona_functions.h
    findOmega();
    
    //Calculating the corona orthonormal tetrad components
	//findTetrad(eTvec, eRvec, eThVec, ePhVec); //in corona_functions.h
    findTetrad();
    //Calculating value of metric components at corona position
    findComponents();//(gTTval, gTPhVal, gRRval, gThThVal, gPhTval, gPhPhVal);
    
    //Calculating trig values at position of corona (used for carter calculation)
    corCos = std::cos(initTheta);
    corSin = std::sin(initTheta);
    corCot = corCos/corSin;
    
    j = procNum*photonPerProc;
    while (j < (procNum+1)*photonPerProc){
      //Generating random angles: alpha (verticle), beta (horizontal)
      imgAlpha = randAlpha();  //Calculating the image verticle angle (alpha)
      imgBeta = randBeta();  //Calculating the image horizontal angle (beta)
      
      //Calculating corona rest frame photon momentum vector
      findMomentum();//(momVec);
      
      //Calculating conserved quantities from B-L momentum vector
      findConserved();//(energy,angmom,carter);
      
      //Resetting the hitDiskSwitch to zero. Changes to 1 if photon hits disk.
      hitDiskSwitch = 0;

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
      vecToOneForm(posVec, momVec, momOneForm);
      
      //Calculating the disk's velocity 4-vector
      diskVelocity(posVec, diskVelVec, scaleHeightValue, rProjected);

      //Calculating the final energy of the photon (-1 x dot product of photon one-form momentum and the disk 4-velocity)
      finalEnergy = (momOneForm[0]*diskVelVec[0]) + (momOneForm[1]*diskVelVec[1]) + (momOneForm[2]*diskVelVec[2]) + (momOneForm[3]*diskVelVec[3]);
      finalEnergy = -1.*finalEnergy;

      //Calculating the gamma (Lorentz factor) of the disk element as seen from LNRF.  This is done by multiplying the 4-vector by the basis one-forms (i.e. the dot product)
      lorentz = findLorentz(posVec,diskVelVec);
      
      //Printing to output file
      myfile << imgAlpha << " " << imgBeta << " " << (finalEnergy/energy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << " " << lorentz << " " << hitDiskSwitch << "\n";
      
      j++;
    }
    myfile.close();  //Closing the output file.
    return 0;  //returning integer value of 0 if executed to completion

}  //end of main function
