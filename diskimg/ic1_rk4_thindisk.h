#include <cmath>

int n = 100; //the number of photons shot per side (total is n x n)
double imgSize = 70.; //length of image plane in rg
double a = 0.; //spin of black hole
double alpha = (M_PI/180.)*9.99971393.; //inclination angle of disk (90 = edge on)
double initRadius = 1000.; //radial position of observer
double initPhi = 0.; //phi position of observer
double initTheta = alpha; //theta position of observer (should equal inclination angle, alpha)
double initTime = 0.; //initial time
double energy = 1.; //initial energy of photons (normalized to 1)
double angmom; //photon phi angular momentum
double carter; //carter constant

double rSqrtSwitch; //switch variable that determines if rdot is negative or positive
double thSqrtSwitch; //switch variable that determines if thetadot is negative or positive

//Defining the spacetime position vector of the photon, setting its components to the initial component
double posVec[4];

//Defining the momentum 4-vector of the photon
double momVec[4];

//Integration variables
double dStep;  //step size, is this even necessary?
double maxStep = 100000000;  //maximal number of steps
double rLimit = (1 + pow(1-(a*a),0.5)) + 0.01;  //minimum radial distance (currently r_horizon + 0.01)
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//Calculating the inner-most stable orbital radius
double z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.)))));
double z2 = sqrt((3.*a*a)+(z1*z1));
double rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));

//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
double efficiencyUpper = (rIsco*rIsco)-(2.*rIsco)+(a*sqrt(rIsco));
double efficiencyLower = (rIsco*rIsco)-(3.*rIsco)+(2.*a*sqrt(rIsco));
double efficiency = 1.-(efficiencyUpper/(rIsco*sqrt(efficiencyLower)));
double accretion = 0.;

//Calculating the front constant for scale height calculation
double heightFrontTerm = 2.*(3./(2.*efficiency))*accretion;
//double heightFrontTerm = (3./(2.*0.1))*accretion;

//Output .txt file name
const char* outFileName = "./euler_test.txt";///n/a3/cjtaylor/raytrace_thindisk/raytrace_data/mcmc_test_a09_i15_acc0.txt";
