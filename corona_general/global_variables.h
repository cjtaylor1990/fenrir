#include <cmath>

//Photon conserved quantities
double energy;
double angmom;
double carter;

//Defining the spacetime position vector of the photon, setting its components to the initial component
double posVec[4];

//Defining the momentum 4-vector of the photon
double momVec[4];

//Defining final momentum one-form for photon
double momOneForm[4];

//Defining energy of photon that is observed co-moving disk frame
double finalEnergy;

//Defining disk velocity variables to be used to calculate gamma
double diskVelVec[4];
double lorentz,lorentzVelSq; //Note that gammaVelSq is the 3-velocity squared and not 4-velocity
double lorentzVelVec[4];

//Defining two distances of interest to be calculated at end of integration
double scaleHeightValue;  //Defining the variable that will hold the scale height where the photon ends up
double rProjected;  //Defining the variable that will hold the radial distance from the final placement of the photon to the black hole, projected onto the plane

//Calculating radius of event horizon
double rEvent = 1. + pow(1.-(a*a),0.5);

//Calculating the inner-most stable orbital radius (r_isco)
double z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.)))));
double z2 = sqrt((3.*a*a)+(z1*z1));
double rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));

//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
double efficiencyUpper = (rIsco*rIsco)-(2.*rIsco)+(a*sqrt(rIsco));
double efficiencyLower = (rIsco*rIsco)-(3.*rIsco)+(2.*a*sqrt(rIsco));
double efficiency = 1.-(efficiencyUpper/(rIsco*sqrt(efficiencyLower)));

//Calculating the front constant for scale height calculation
double heightFrontTerm = 2.*(3./(2.*efficiency))*accretion;

//Position of the corona in B-L coordinates
double initTime,initRadius,initTheta,initPhi;

//Corona rotational velocity (dphi/dt = rotOmega)
double rotOmega;

//Integration variables
double dStep;  //step size; changed sign
double rLimitLow = rEvent + horizonStop;  //minimum radial distance (currently r_horizon + 0.1) (changed to 0.5)

double rSqrtSwitch;
double thSqrtSwitch;
double hitDiskSwitch;

//Angles in corona co-moving frame
double imgAlpha,imgBeta;  //Defining the angle variables

//Basis vectors for orthonormal tetrad at corona
double eTvec[4],eRvec[4],eThVec[4],ePhVec[4];