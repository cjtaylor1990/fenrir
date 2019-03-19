#include <cmath>

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
