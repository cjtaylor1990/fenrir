#include <iostream>
#include <cmath>


//This is a function that will take in the initial position vector (posVec) and momentum vector (momVec) and integrate them forward, until rLimit or disk is reached, or it dies after so many steps

void propagate(double posVec[4], double momVec[4], double dStep, double tolerance, double maxStep, double rLimit, double rEvent, double scaleHeightValue, double rProjected){
    //Space components are centered at black hole.
	double time = posVec[0];  //time spacetime component
    double radius = posVec[1];  //radial spacetime component
    double theta = posVec[2];  //theta spacetime component (angle between z-axis and 3-vector)
	double phi = posVec[3];  //phi spacetime component (angle between 3-vector projection in x-y plane and x-axis)

    //Defining k-arrays, each entry a seperate term for the RK4 integration
    double kTarray[4];
    double kRsqArray[4];
    double kRarray[4];
    double kThSqArray[4];
    double kThArray[4];
    double kPhiArray[4];

    //Defining step to be taken per dStep
    double tStep;
    double rStep;
    double thStep;
    double phiStep;

    //defining integration counter i
	int i = 0;

    //starting main processing loop.  Will continue while i is less than the maximum number of steps and the radial distance is less than the minimal value specified
	while ((i < maxStep) && (radius > rLimit) && (radius*cos(theta) > scaleHeightFnct(radius,theta))){
        //First set of k's for rk4 integration
        kTarray[0] = tdot(radius,phi,theta,time);
        kRsqArray[0] = rdotsq(radius,phi,theta,time);
        kThSqArray[0] = thdotsq(radius,phi,theta,time);
        kPhiArray[0] = phidot(radius,phi,theta,time);

        //Checking if rdot^2 is at a minimal value at 0, in which case, the sign of rdot must be flipped
        if (kRsqArray[0] < 0.){
            rSqrtSwitch = -1.*rSqrtSwitch;  //flipping the sign of rSqrtSwitch
            //std::cout << "PING R! " << i << "\n";
        }
        //Checking if thetadot^2 is at a minimal value at 0, in which case, the sign of thetadot must be flipped
        if (kThSqArray[0] < 0.){
            thSqrtSwitch = -1.*thSqrtSwitch;  //flipping the sign of thSqrtSwitch
            //std::cout << "PING THETA! " << i << "\n";
        }

        kRarray[0] = rSqrtSwitch*sqrt(std::fabs(kRsqArray[0]));
        kThArray[0] = thSqrtSwitch*sqrt(std::fabs(kThSqArray[0]));

        //Calculating step size (auto-refinement)
        dStep = ((radius - rEvent)/(std::fabs(kRarray[0])*tolerance));
        if (dStep > (1./(std::fabs(kThArray[0]*tolerance)))){
            dStep = 1./(std::fabs(kThArray[0])*tolerance);
        }
        if (dStep > (1./(std::fabs(kPhiArray[0]*tolerance)))){
            dStep = 1./(std::fabs(kPhiArray[0])*tolerance);
        }
        dStep = -1.*dStep;

        //Second set of k's for rk4 integration
        kTarray[1] = tdot(radius+((dStep/2.)*kRarray[0]),phi+((dStep/2.)*kPhiArray[0]),theta+((dStep/2.)*kThArray[0]),time+((dStep/2.)*kTarray[0]));
        kRsqArray[1] = rdotsq(radius+((dStep/2.)*kRarray[0]),phi+((dStep/2.)*kPhiArray[0]),theta+((dStep/2.)*kThArray[0]),time+((dStep/2.)*kTarray[0]));
        kThSqArray[1] = thdotsq(radius+((dStep/2.)*kRarray[0]),phi+((dStep/2.)*kPhiArray[0]),theta+((dStep/2.)*kThArray[0]),time+((dStep/2.)*kTarray[0]));
        kPhiArray[1] = phidot(radius+((dStep/2.)*kRarray[0]),phi+((dStep/2.)*kPhiArray[0]),theta+((dStep/2.)*kThArray[0]),time+((dStep/2.)*kTarray[0]));

        //Checking if rdot^2 is at a minimal value at 0, in which case, the sign of rdot must be flipped
        if (kRsqArray[1] < 0.){
            rSqrtSwitch = -1.*rSqrtSwitch;  //flipping the sign of rSqrtSwitch
            //std::cout << "PING R! " << i << "\n";
        }
        //Checking if thetadot^2 is at a minimal value at 0, in which case, the sign of thetadot must be flipped
        if (kThSqArray[1] < 0.){
            thSqrtSwitch = -1.*thSqrtSwitch;  //flipping the sign of thSqrtSwitch
            //std::cout << "PING THETA! " << i << "\n";
        }

        kRarray[1] = rSqrtSwitch*sqrt(std::fabs(kRsqArray[1]));
        kThArray[1] = thSqrtSwitch*sqrt(std::fabs(kThSqArray[1]));

        //Third set of k's for rk4 integration
        kTarray[2] = tdot(radius+((dStep/2.)*kRarray[1]),phi+((dStep/2.)*kPhiArray[1]),theta+((dStep/2.)*kThArray[1]),time+((dStep/2.)*kTarray[1]));
        kRsqArray[2] = rdotsq(radius+((dStep/2.)*kRarray[1]),phi+((dStep/2.)*kPhiArray[1]),theta+((dStep/2.)*kThArray[1]),time+((dStep/2.)*kTarray[1]));
        kThSqArray[2] = thdotsq(radius+((dStep/2.)*kRarray[1]),phi+((dStep/2.)*kPhiArray[1]),theta+((dStep/2.)*kThArray[1]),time+((dStep/2.)*kTarray[1]));
        kPhiArray[2] = phidot(radius+((dStep/2.)*kRarray[1]),phi+((dStep/2.)*kPhiArray[1]),theta+((dStep/2.)*kThArray[1]),time+((dStep/2.)*kTarray[1]));

        //Checking if rdot^2 is at a minimal value at 0, in which case, the sign of rdot must be flipped
        if (kRsqArray[2] < 0.){
            rSqrtSwitch = -1.*rSqrtSwitch;  //flipping the sign of rSqrtSwitch
            //std::cout << "PING R! " << i << "\n";
        }
        //Checking if thetadot^2 is at a minimal value at 0, in which case, the sign of thetadot must be flipped
        if (kThSqArray[2] < 0.){
            thSqrtSwitch = -1.*thSqrtSwitch;  //flipping the sign of thSqrtSwitch
            //std::cout << "PING THETA! " << i << "\n";
        }

        kRarray[2] = rSqrtSwitch*sqrt(std::fabs(kRsqArray[2]));
        kThArray[2] = thSqrtSwitch*sqrt(std::fabs(kThSqArray[2]));

        //Final set of k's for rk4 integration
        kTarray[3] = tdot(radius+((dStep)*kRarray[2]),phi+((dStep)*kPhiArray[2]),theta+((dStep)*kThArray[2]),time+((dStep)*kTarray[2]));
        kRsqArray[3] = rdotsq(radius+((dStep)*kRarray[2]),phi+((dStep)*kPhiArray[2]),theta+((dStep)*kThArray[2]),time+((dStep)*kTarray[2]));
        kThSqArray[3] = thdotsq(radius+((dStep)*kRarray[2]),phi+((dStep)*kPhiArray[2]),theta+((dStep)*kThArray[2]),time+((dStep)*kTarray[2]));
        kPhiArray[3] = phidot(radius+((dStep)*kRarray[2]),phi+((dStep)*kPhiArray[2]),theta+((dStep)*kThArray[2]),time+((dStep)*kTarray[2]));

        //Checking if rdot^2 is at a minimal value at 0, in which case, the sign of rdot must be flipped
        if (kRsqArray[3] < 0.){
            rSqrtSwitch = -1.*rSqrtSwitch;  //flipping the sign of rSqrtSwitch
            //std::cout << "PING R! " << i << "\n";
        }
        //Checking if thetadot^2 is at a minimal value at 0, in which case, the sign of thetadot must be flipped
        if (kThSqArray[3] < 0.){
            thSqrtSwitch = -1.*thSqrtSwitch;  //flipping the sign of thSqrtSwitch
            //std::cout << "PING THETA! " << i << "\n";
        }

        kRarray[3] = rSqrtSwitch*sqrt(std::fabs(kRsqArray[3]));
        kThArray[3] = thSqrtSwitch*sqrt(std::fabs(kThSqArray[3]));

        //Calculating the amount the that each spacetime component will advance by for each total step (RK4)
        tStep = (dStep/6.)*(kTarray[0]+(2.*kTarray[1])+(2.*kTarray[2])+kTarray[3]);
        rStep = (dStep/6.)*(kRarray[0]+(2.*kRarray[1])+(2.*kRarray[2])+kRarray[3]);
        thStep = (dStep/6.)*(kThArray[0]+(2.*kThArray[1])+(2.*kThArray[2])+kThArray[3]);
        phiStep = (dStep/6.)*(kPhiArray[0]+(2.*kPhiArray[1])+(2.*kPhiArray[2])+kPhiArray[3]);


        //Integration step of spacetime components (currently: RK4)
		radius += rStep;
		phi += phiStep;
		theta += thStep;
		time += tStep;
        //std::cout << time << " " << radius << " " << theta << " " << phi << "\n";

        i++;  //incrementing the integration counter (i.e. setting i = i+1)
		}
	//Filling in the components to the position 4-vector
    posVec[0] = time;
    posVec[1] = radius;
    posVec[2] = theta;
    posVec[3] = phi;

    //Filling in the components to the momentum 4-vector
    momVec[0] = tStep/dStep;
    momVec[1] = rStep/dStep;
    momVec[2] = thStep/dStep;
    momVec[3] = phiStep/dStep;
}  //end of main function
