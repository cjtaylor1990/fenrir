#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "ic1_multi_new.h"
#include "variable_names.h"
#include "photon_geos_thindisk.h"
#include "metric_kerr.h"
#include "disk_equations_new.h"
#include "propagate_rk4_thindisk_new.h"

//Last update by Corbin Taylor at 11:28 am on 3/27/19

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


int main(int argc, char* argv[]){
	
	//Getting total number of photons and rows/process from command line
	//This divides the vertical dimension into parts, where each process does a certain number of rows
	int n = atoi(argv[1]); //total number of rows/columns for disk image (n x n photons)
	int totProcs = atoi(argv[2]); //total number of processes
	int procNum = atoi(argv[3]); //index number of process (starts at 0)
	int colPerProc = n/totProcs; //calculating columns per parallel process
    
    //Physical parameters
    a = atof(argv[4]);//0.0;
	inclination = acos(atof(argv[5]));//15.0;
	
	//Accretion parameters (initAcc > finalAcc)
	numThickness = atoi(argv[6]);
	initThickness = atof(argv[7]);
	finalThickness = atof(argv[8]);
	imgSize = atof(argv[9]);
	outFilePrefix = argv[10];
	
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
	rLimit = rEvent + horizonStop;  //minimum radial distance (currently r_horizon + 0.01) //
	
	//Setting up the initial photon position
	initRadius = obsRadius; //radial position of observer
	initPhi = obsPhi; //phi position of observer
	initTheta = inclination; //theta position of observer (should equal inclination angle, alpha)
	initTime = 0.; //initial time
	

	//Initializing the array of output streams
	std::ofstream outStreams[numThickness];
	
	//Initializing the array of stream names
	//const char* outNames[numAcc];
	
	//Filling stream name array
	//int fileIndex = 0;
	//while (fileIndex < numAcc){
	//	outNames[fileIndex] = outFilePrefix + ;
	//	fileIndex += 1;
	//}
	int fileIndex = 0;
	
	//Opening up each of the output streams by looping over output stream array outStreams
	//int fileIndex = 0;
	std::string fileName;
	while (fileIndex < numThickness){
		fileName = outFilePrefix + "_" + patch::to_string(fileIndex) + ".txt";
		outStreams[fileIndex].open(fileName.c_str());
		fileIndex += 1;
	}
	
	//Setting accretion rate to the initial accretion value (initAcc)
	//accretion = initAcc;
	
	//Defining the front normalization term of the disk height equation (above the mid-plane)
	//heightFrontTerm = 2.*(3./(2.*efficiency))*accretion; //Calculating the front term of the scale height equation //
	
    //Looping over NxN image plane
    int j,k; //defining looping dummy variables. j is x variable. k is y variable
    
    //Accretion rate dummy variable
    int thicknessIndex;

    j = procNum*colPerProc; //setting dummy j to 0
    while (j < (procNum+1)*colPerProc){
        k = 0;//procNum*rowsPerProc; //resetting k dummy variable to starting value
        while (k < n){//(procNum+1)*rowsPerProc){
            //std::cout << j << " " << k << "\n";
            imgX = ((j - (n/2.))+0.5)*(imgSize/n); //calculating x value for j value, could always do it outside of loop to not have to constantly do calculation
            imgY = ((k - (n/2))+0.5)*(imgSize/n); //calculating y value for k value
            imgB = pow(pow(imgX,2.)+pow(imgY,2.),0.5); //calculating Euclidean 2-D distance on image plane

            //calculating the phi angular momentum and carter constant of photon from image plane coordinates
            angmom = -1.*imgB*std::sin(inclination)*(imgX/imgB); //phi angular momentum
            carter = pow(imgB*(imgY/imgB),2.) - energy*pow(a*std::cos(initTheta),2.) + pow(angmom*std::cos(initTheta)/std::sin(initTheta), 2.); //carter constant

			//setting the switches for the sign of rdot and thetadot
            rSqrtSwitch = 1.;
            if (imgY < 0.){
                thSqrtSwitch = -1.;
            }else{
                thSqrtSwitch = 1.;
            }

			//setting the initial values of the photon's position 4-vector to be found in the ic file
            posVec[0] = initTime; //time
            posVec[1] = initRadius; //spherical radius
            posVec[2] = initTheta; //vertical angle theta
            posVec[3] = initPhi; //horizontal angle phi
            
			//Starting the propagation loop, where I will write to a different file for each Mdot value
			thicknessIndex = 0;
			while (thicknessIndex < numThickness){
				//Calculating the accretion rate
				thickness = initThickness + (thicknessIndex*deltaThickness);
				
				//Calculating the scale height normalization term from the accretion rate
				heightFrontTerm = thickness;//2.*(3./(2.*efficiency))*accretion;
				
				if (posVec[1]*cos(posVec[2]) > scaleHeightFnct(posVec[1],posVec[2])){
				
            		//Propagate the photon from the observer to the disk
            		propagate(posVec,momVec,dStep,tolerance,maxStep,rLimit,rEvent,scaleHeightValue,rProjected);
				
				}
            	//Calculating the pseudo-cylindrical radius and the vertical height of the disk above the mid-plane
            	rProjected = posVec[1]*std::sin(posVec[2]); //pseudo-cylindrical radius
            	scaleHeightValue = posVec[1]*std::cos(posVec[2]);//heightFrontTerm*(1 - sqrt(rIsco/rProjected)); //vertical height of the disk above mid-plane

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
