#include <cmath>

//Calculating radius of event horizon
double rEvent = 1. - pow(1.-(a*a),0.5);

//Calculating the inner-most stable orbital radius
double z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.)))));
double z2 = sqrt((3.*a*a)+(z1*z1));
double rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));

//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
double efficiencyUpper = (rIsco*rIsco)-(2.*rIsco)+(a*sqrt(rIsco));
double efficiencyLower = (rIsco*rIsco)-(3.*rIsco)+(2.*a*sqrt(rIsco));
double efficiency = 1.-(efficiencyUpper/(rIsco*sqrt(efficiencyLower)));

//Disk thickness variables
double scaleHeightValue; //vertical height of disk above the midplane
double heightFrontTerm = 2.*(3./(2.*efficiency))*accretion; //Calculating the front term of the scale height equation

//Initial position of photons (at observer)
double initRadius = obsRadius; //radial position of observer
double initPhi = obsPhi; //phi position of observer
double initTheta = inclination; //theta position of observer (should equal inclination angle, alpha)
double initTime = 0.; //initial time

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
double rLimit = rEvent + horizonStop;  //minimum radial distance (currently r_horizon + 0.01)
