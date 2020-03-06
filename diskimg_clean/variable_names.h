#include <cmath>

//Phystical parameters
double a;// = 0.0;
double inclination;// = (M_PI/180.)*15.0;

//Accretion parameters
int numThickness; //the number of Mdot values being explored
double initThickness; //the first Mdot value being explored
double finalThickness; //the final Mdot being explored
double deltaThickness; //the change in Mdot between the different Mdot values
double thickness; //the current mass accretion rate

//Position of observer
double obsRadius = 1000.; //radius of observer
double obsPhi = 0.; //phi angle of observer

//Integration variables
//int n;// = 500; //the number of photons shot per side (total is n x n)
double imgSize; //length of image plane in rg
double maxStep = 100000000;  //maximal number of steps
double horizonStop = 0.01;  //minimum radial distance from event horizon before considered lost
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//The prefix for the different output files
std::string outFilePrefix;// = "./test_new.txt";///n/a3/cjtaylor/raytrace_thindisk/raytrace_data/mcmc_test_a09_i15_acc0.txt";

//Calculating radius of event horizon
double rEvent; //

//Calculating the inner-most stable orbital radius
double z1; //
double z2; //
double rIsco; //

//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
double efficiencyUpper; //
double efficiencyLower; //
double efficiency; //

//Disk thickness variables
double scaleHeightValue; //vertical height of disk above the midplane
double heightFrontTerm; //Calculating the front term of the scale height equation //

//Initial position of photons (at observer)
double initRadius; //radial position of observer
double initPhi; //phi position of observer
double initTheta; //theta position of observer (should equal inclination angle, alpha)
double initTime; //initial time

//Cylindrical radius of photon
double rProjected; //pseudo-cylindrical radius

//Photon conserved quantities
double energy = 1.; //initial energy of photons (normalized to 1)
double angmom; //photon phi angular momentum
double carter; //carter constant

//Energy of photon seen in the co-moving frame of the disk
double finalEnergy;

//Defining the spacetime position vector of the photon, setting its components to the initial component
double posVec[4];

//Defining the momentum 4-vector and one-form of the photon
double momVec[4];
double momOneForm[4];

//Defining the disk 4-velocity
double diskVelVec[4]; //defining disk's 4-velocity

//Defining photon grid to be "shot at the disk"
double imgX,imgY,imgB; //defining x,y cartesian grid for image plane. imgX = x, imgY = y, and imgB = 2-D distance (sqrt(x^2 + y^2))

//Defining integration variables
double dStep; //step size variable that will be refined over integration
double rSqrtSwitch; //switch variable that determines if rdot is negative or positive
double thSqrtSwitch; //switch variable that determines if thetadot is negative or positive
double rLimit;  //minimum radial distance (currently r_horizon + 0.01) //
