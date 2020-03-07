#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "userSettings.hpp"
#include "metric.hpp"
#include "photon.hpp"
#include "disk.hpp"
#include "observer.hpp"
#include "integrator.hpp"
#include "stringPatch.hpp"

int main(int argc, char* argv[]){
	
	//Photon number and parallelization information
	int n = atoi(argv[1]); //total number of rows/columns for disk image (n x n photons)
	int totProcs = atoi(argv[2]); //total number of processes
	int procNum = atoi(argv[3]); //index number of process (starts at 0)
	int colPerProc = n/totProcs; //calculating columns per parallel process
	
	//Physical parameters
	double spin = atof(argv[4]); //black hole spin (dimensionless)
	double inclination = acos(atof(argv[5])); //disk inclination (degrees)
	
	//Accretion parameters (initAcc > finalAcc)
	int numThickness = atoi(argv[6]);
	int initThickness = atof(argv[7]);
	int finalThickness = atof(argv[8]);
	int imgSize = atof(argv[9]);
	std::string outFilePrefix = argv[10];
	
	//Calculating the deltaAcc from the inputs
	double deltaThickness;
	if (numThickness > 1){
		deltaThickness = (finalThickness - initThickness)/(numThickness-1);
	}else{
		deltaThickness = 0.;
	}
	
	//Instantiating the Metric
	Metric metric(spin);

	//Constructing the inital position vector (place of observer)
	double initPosition[4] = {userSettings::initTime, userSettings::initRadius, inclination, userSettings::initPhi};
	
	//Instantiating the observer
	Observer observer(initPosition, imgSize, n);
	
	//Initializing the array of output streams
	std::ofstream outStreams[numThickness];
	
	//Opening up each of the output streams by looping over output stream array outStreams
	std::string fileName;
	int fileIndex = 0;
	while (fileIndex < numThickness){
		fileName = outFilePrefix + "_" + stringPatch::toString(fileIndex) + ".txt";
		outStreams[fileIndex].open(fileName.c_str());
		fileIndex += 1;
	}
	
	//Looping over NxN image plane
	int j,k; //defining looping dummy variables. j is x variable. k is y variable
	
	//Accretion rate dummy variable
	int thicknessIndex;

	j = procNum*colPerProc; //setting dummy j (x index)
	while (j < (procNum+1)*colPerProc){
		k = 0; //setting dummy k (yIndex)
		while (k < n){

			double photonImgX = observer.imgX(j); //calculating x value for j value
			double photonImgY = observer.imgY(k); //calculating y value for k value

			//calculating the energy, phi angular momentum, and carter constant of photon from image plane coordinates
			double photonEnergy = observer.energy(metric, j, k);
			double photonAngMomentum = observer.angMomentum(metric, j, k);
			double photonCarter = observer.carter(metric, j, k);

			//setting the switches for the sign of rdot and thetadot
			double photonRswitch = observer.rSwitch(j, k);
			double photonThetaSwitch = observer.thetaSwitch(j, k);

			//instantiating the photon
			Photon photon(initPosition, photonEnergy, photonAngMomentum, photonCarter, photonRswitch, photonThetaSwitch);
			
			//Instantiating the disk with the initial thickness
			Disk disk(metric, initThickness);

			//Instantiating the integrator
			Integrator integrator(metric, userSettings::tolerance, userSettings::maxStepNumber, userSettings::horizonStop);

			//Starting the propagation loop, where I will write to a different file for each Mdot value
			thicknessIndex = 0;
			while (thicknessIndex < numThickness){
				//Setting the disk thickness to current value
				double currentThickness = initThickness + (thicknessIndex*deltaThickness);
				disk.updateDiskThickness(currentThickness);
				
				if (integrator.hasHitDisk(photon, disk) {
				
					//Propagate the photon from the observer to the disk
					integrator.propagate(metric, photon, disk);
				
				}

				//Calculating the pseudo-cylindrical radius and the vertical height of the disk above the mid-plane
				rProjected = cylindricalR({photon.time(), photon.radius(), photon.theta(), photon.phi()}); 
				scaleHeightValue = disk.scaleHeight({photon.time(), photon.radius(), photon.theta(), photon.phi()});

				//Calculating the one-form of the photon's momentum 4-vector
				vecToOneForm(posVec, momVec, momOneForm);

				//Calculating the disk's velocity 4-vector
				diskVelocity(posVec, diskVelVec, scaleHeightValue, rProjected);
			
				//Calculating the final energy of the photon (E = -p*U, dotting photon 4-momentum with disk 4-velocity)
				finalEnergy = (momOneForm[0]*diskVelVec[0]) + (momOneForm[1]*diskVelVec[1]) + (momOneForm[2]*diskVelVec[2]) + (momOneForm[3]*diskVelVec[3]);
				finalEnergy = -1.*finalEnergy;

				//Outputting data to 'myfile': x, y, g, final_t, final_r, final_theta, final_phi, disk_H, pseudo-cylindrical_r
				outStreams[thicknessIndex] << imgX << " " << imgY << " " << (energy/finalEnergy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << "\n";
				
				//advancing the accretion index by 1
				thicknessIndex += 1;
			}
			//advancing k by 1
			k++;
		}
		//advancing j by 1
		j++;
	}
	//closing the various output files to be saved by looping over the stream array
	fileIndex = 0;
	while (fileIndex < numThickness){
		outStreams[fileIndex].close();
		fileIndex += 1;
	}
	return 0;  //returning integer value of 0 if executed to completion

}  //end of main function
