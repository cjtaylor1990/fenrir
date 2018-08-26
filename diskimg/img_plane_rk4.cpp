#include <iostream>
#include <fstream>
#include <cmath>
#include "ic1_rk4_thindisk.h"
#include "variable_names.h"
#include "metric_kerr.h"
#include "photon_geos_thindisk.h"
#include "disk_equations.h"
#include "propagate_rk4_thindisk.h"


int main(int argc, char* argv[]){
    //Opening file that will be the output
    std::ofstream myfile; //opening i/o stream to output to 'myfile' named in ic file
    myfile.open (outFileName); //opening a new txt file 'myfile' named in ic file

    //Looping over NxN image plane
    int j,k; //defining looping dummy variables. j is x variable. k is y variable

    //Setting sign of spinTermDisk (switch for angular velocity)
    if (a < 0){
      spinTermDisk = -1.;
    }
    else{
      spinTermDisk = 1.;
    }

    j = 0; //setting dummy j to 0
    while (j < n){
        k = 0; //setting dummy k to 0 (gets set back to 0 for each j loop)
        while (k < n){
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

            //Propagate the photon from the observer to the disk
            propagate(posVec,momVec,dStep,tolerance,maxStep,rLimit,rEvent,scaleHeightValue,rProjected);

            //Calculating the pseudo-cylindrical radius and the vertical height of the disk above the mid-plane
            rProjected = posVec[1]*std::sin(posVec[2]); //pseudo-cylindrical radius
            scaleHeightValue = heightFrontTerm*(1 - sqrt(rIsco/rProjected)); //vertical height of the disk above mid-plane

            //Calculating the one-form of the photon's momentum 4-vector
            vecToOneForm(posVec, momVec, momOneForm);

            //Calculating the disk's velocity 4-vector
            diskVelVec[0] = diskTdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //time
            diskVelVec[1] = diskRdot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //radius
            diskVelVec[2] = diskThDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //theta
            diskVelVec[3] = diskPhiDot(posVec[1],posVec[3],posVec[2],posVec[0],scaleHeightValue,rProjected); //phi

            //Calculating the final energy of the photon (E = -p*U, dotting photon 4-momentum with disk 4-velocity)
            finalEnergy = (momOneForm[0]*diskVelVec[0]) + (momOneForm[1]*diskVelVec[1]) + (momOneForm[2]*diskVelVec[2]) + (momOneForm[3]*diskVelVec[3]);
            finalEnergy = -1.*finalEnergy;

			//Outputting data to 'myfile': x, y, g, final_t, final_r, final_theta, final_phi, disk_H, pseudo-cylindrical_r
            myfile << imgX << " " << imgY << " " << (energy/finalEnergy) << " " << posVec[0] << " " << posVec[1] << " " << posVec[2] << " " << posVec[3] << " " << scaleHeightValue << " " << rProjected << "\n";

            //advancing k by 1
            k++;
        }
        //advancing j by 1
        j++;
    }
    //closing 'myfile' to be saved in path defined in ic file
    myfile.close();
    return 0;  //returning integer value of 0 if executed to completion

}  //end of main function
