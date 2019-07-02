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

//Need to convert this from defining the disk by theta to vertal distance from theta = pi/2

//This allows me to convert an integer into a string, overcoming the version problems on galaxy (outdated icc compiler)
#include <string>
#include <sstream>
namespace patch
{
    template < typename T > std::string to_string( const T& nInput )
    {
        std::ostringstream stm ;
        stm << nInput ;
        return stm.str() ;
    }
}

//main function
int main(int argc, char* argv[]){

	//This divides the job over multiple processes
	int n, totProcs, procNum, photonPerProc;
	n = atoi(argv[1]); //total number of photons traced from corona
	totProcs = atoi(argv[2]); //total number of process
	procNum = atoi(argv[3]); //index number of current process (starts at 0)
	photonPerProc = n/totProcs; //number of photons per process

	//Accessing user-input for spin (a) and corona height (height)
	a = atof(argv[4]);
	height = atof(argv[5]);

	//Accretion parameters (initAcc > finalAcc)
	numThickness = atoi(argv[6]); //Total number of thickness values
	initThickness = atof(argv[7]); //Initial thickness that each process will consider
	finalThickness = atof(argv[8]); //Final thickness that each process will consider

	//Output file information
	outFilePrefix = argv[9]; //Prefix of input file (file name: outputFilePrefix_thicknessIndex_processIndex.txt)

	//Calculating the deltaAcc from the inputs
	if (numThickness > 1){
		deltaThickness = (finalThickness - initThickness)/(numThickness-1);
	}else{
		deltaThickness = 0.;
	}

	//Calculating radius of event horizon
	rEvent = 1. + pow(1.-(a*a),0.5); //

	//Calculating the inner-most stable orbital radius
	z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.))))); //
	z2 = sqrt((3.*a*a)+(z1*z1)); //
	if (a < 0.){
		rIsco = 3.+z2+sqrt((3-z1)*(3+z1+(2.*z2))); //
	}else{
		rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));
	}

	//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
	efficiencyUpper = (rIsco*rIsco)-(2.*rIsco)+(a*sqrt(rIsco)); //
	efficiencyLower = (rIsco*rIsco)-(3.*rIsco)+(2.*a*sqrt(rIsco)); //
	efficiency = 1.-(efficiencyUpper/(rIsco*sqrt(efficiencyLower))); //

	//Defining the minimum R cutoff (at which point, the photon will stop being traced)
	rLimitLow = rEvent + horizonStop;  //minimum radial distance (currently r_horizon + 0.01) //

	//Initializing the array of output streams
	std::ofstream outStreams[numThickness];

	//Opening up each of the output streams by looping over output stream array outStreams
	int fileIndex = 0;
	std::string fileName;
	while (fileIndex < numThickness){
		fileName = outFilePrefix + "_" + patch::to_string(fileIndex) + "_" + patch::to_string(procNum) + ".txt";
		outStreams[fileIndex].open(fileName.c_str());
		fileIndex += 1;
	}

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

	//Angular griding for Dauser emissivity method
	double minAngle = ((a*a)*((initRadius*initRadius)-(2.*initRadius)+(a*a)))/(((initRadius*initRadius)+(a*a))*((initRadius*initRadius)+(a*a)));
	double maxAngle = M_PI-minAngle;
	double delAngle = (maxAngle - minAngle)/(n-1);

	//The angles aren't right for parallelization, but that's okay for now. Need to fix it.
	//Dummy variable for main photon tracing loop (each iteration is new photon)
 	int j = procNum*photonPerProc;

	//Accretion rate dummy variable (will change each time you hit a different disk level throughout a single j loop)
	int thicknessIndex;

 	while (j < (procNum+1)*photonPerProc){
  		//Generating initial angle of photon path relative to polar azis
  		imgAlpha = M_PI - (minAngle + j*delAngle);//randAlpha();  //Calculating the image verticle angle (alpha)
  		imgBeta = 0.; //Due to azimuthal symmetry in LP case, I can choose one beta value.

		//Calculating corona rest frame photon momentum vector
		findMomentum();//(momVec);

		//Calculating conserved quantities from B-L momentum vector
      	findConserved();//(energy,angmom,carter);

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

	  //Starting the propagation loop, where I will write to a different file for each Mdot value
	  thicknessIndex = 0;
	  while (thicknessIndex < numThickness){

		  //Calculating the accretion rate
		  thickness = initThickness + (thicknessIndex*deltaThickness);

		  //Calculating the scale height normalization term from the accretion rate
		  heightFrontTerm = thickness;//2.*(3./(2.*efficiency))*accretion;

		  if (posVec[1]*cos(posVec[2]) > scaleHeightFnct(posVec[1],posVec[2])){
			  //Resetting the hitDiskSwitch to zero. Changes to 1 if photon hits disk.
			  hitDiskSwitch = 0;

			  //Propagate the photon to the disk
		      propagate(posVec,momVec,dStep,tolerance,maxStep,rLimitLow,rLimitHigh,rEvent,scaleHeightValue,rProjected);

		  }

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
		  outStreams[thicknessIndex] << imgAlpha << " " << imgBeta << " " << (finalEnergy/energy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << " " << lorentz << " " << hitDiskSwitch << "\n";

		  //advancing the accretion index by 1
		  thicknessIndex += 1;

	  	}
	//Incrementing j index (changing to next photon)
		j++;
	}
	//closing the various output files to be saved by looping over the stream array
	fileIndex = 0;
	while (fileIndex < numThickness){
		outStreams[fileIndex].close();
		fileIndex += 1;
	}

	return 0;  //returning integer value of 0 if executed to completion

} //end of main function
