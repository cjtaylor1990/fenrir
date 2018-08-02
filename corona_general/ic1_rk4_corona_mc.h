#include <cmath>

int n = 1000000.;
//double imgSize = 100.;
double a = 0.998;
//double alpha = (M_PI/180.)*0.01;
double initCylRadius = 5.;
double heightAboveDisk = 1.;
double rotOmega = 1./(pow(initCylRadius,1.5) + a);
double energy;
double angmom;
double carter;

double rSqrtSwitch;
double thSqrtSwitch;
double hitDiskSwitch;

//Defining the spacetime position vector of the photon, setting its components to the initial component
double posVec[4];

//Defining the momentum 4-vector of the photon
double momVec[4];

//Integration variables
double dStep;  //step size; changed sign
double maxStep = 100000000;  //maximal number of steps
double rLimitLow = (1 + pow(1-(a*a),0.5)) + 0.01;  //minimum radial distance (currently r_horizon + 0.1) (changed to 0.5)
double rLimitHigh = 5000.;
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//Calculating the inner-most stable orbital radius
double z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.)))));
double z2 = sqrt((3.*a*a)+(z1*z1));
double rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));

//Efficiency and accretion rate (Mdot/Eddington) to calculate scale height
double efficiencyUpper = (rIsco*rIsco)-(2.*rIsco)+(a*sqrt(rIsco));
double efficiencyLower = (rIsco*rIsco)-(3.*rIsco)+(2.*a*sqrt(rIsco));
double efficiency = 1.-(efficiencyUpper/(rIsco*sqrt(efficiencyLower)));
double accretion = 0.3;

//Calculating the front constant for scale height calculation
double heightFrontTerm = 2.*(3./(2.*efficiency))*accretion;
//double heightFrontTerm = (3./(2.*0.1))*accretion;

//Calculating the position of corona in B-L coordinates
double thicknessAtCorona = heightFrontTerm*(1. - sqrt(rIsco/initCylRadius));
double initTheta = std::atan(initCylRadius/(thicknessAtCorona+heightAboveDisk));
double initRadius = initCylRadius/std::sin(initTheta);
double initPhi = 0.;//M_PI;
double initTime = 0.;

//Output .txt file name
const char* outFileName = "./test.txt";//"/n/a3/cjtaylor/raytrace_thindisk/raytrace_data/coronalp_a095_acc03_r3_tol500_n1e6.txt";
