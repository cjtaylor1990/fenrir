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
				
				if (integrator.hasHitDisk(photon, disk)) {
				
					//Propagate the photon from the observer to the disk
					integrator.propagate(metric, photon, disk);
				
				}

				double finalPosition[4] = {photon.time(), photon.radius(), photon.theta(), photon.phi()};
				double finalMomentum[4];
				photon.getMomentum(finalMomentum);

				//Calculating the pseudo-cylindrical radius and the vertical height of the disk above the mid-plane
				double rProjected = cylindricalR(finalPosition); 
				double scaleHeightValue = disk.scaleHeight(finalPosition);

				//Calculating the one-form of the photon's momentum 4-vector
				double momentumOneForm[4];
				metric.vectorToOneForm(finalPosition, finalMomentum, momentumOneForm);

				//Calculating the disk's velocity 4-vector
				double diskVelocity[4] = {disk.diskTdot(metric, finalPosition), disk.diskRdot(metric, finalPosition), disk.diskThDot(metric, finalPosition), disk.diskPhiDot(metric, finalPosition)};
			
				//Calculating the final energy of the photon (E = -p*U, dotting photon 4-momentum with disk 4-velocity)
				double finalEnergy = (momentumOneForm[0]*diskVelocity[0]) + (momentumOneForm[1]*diskVelocity[1]) + (momentumOneForm[2]*diskVelocity[2]) + (momentumOneForm[3]*diskVelocity[3]);
				finalEnergy = -1.*finalEnergy;

				//Outputting data to 'myfile': x, y, g, final_t, final_r, final_theta, final_phi, disk_H, pseudo-cylindrical_r
				outStreams[thicknessIndex] << photonImgX << " " << photonImgY << " " << (observer.energy(metric, photonImgX, photonImgY)/finalEnergy) << " " << finalPosition[0] << " " << finalPosition[1] << " " << finalPosition[2] << " " << finalPosition[3] << " " << scaleHeightValue << " " << rProjected << "\n";
				
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
