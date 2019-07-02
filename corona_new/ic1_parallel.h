#include <cmath>

//int n;
//int phot_per_proc;

double a;// = 0.998;
double accretion = 0.0;
std::string coronaType = "lp"; //can be equal to 'lp' (lamppost) or 'offaxis' (offaxis corona)

//Lamppost variables. Need if coronaType = 'lp'
double height;// = 10.;  //lamppost corona height

//Accretion parameters
int numThickness; //the number of Mdot values being explored
double initThickness; //the first Mdot value being explored
double finalThickness; //the final Mdot being explored
double deltaThickness; //the change in Mdot between the different Mdot values
double thickness; //the current mass accretion rate

//Off-axis variables. Need if coronaType = 'offaxis'.
double initCylRadius = 1.; //cylindrical radius of corona
double heightAboveDisk = 0.1; //corona vertical distance above disk surface
double coronaPhi = 0.; //phi position of corona
bool isRisco = true; //tells Fenrir that initCylRadius is in units of r_ISCO
std::string rotOmegaType = "kep"; //Can be a value (dphi/dt) or the string 'lnrf' or 'kep'.
double rotOmegaInput;

//Seedof the random number generator
double seed = time(NULL);

//Integration variables
double maxStep = 100000000;  //Maximal number of steps
double horizonStop = 0.01; //Minimum distance from photon to event horizon before being lost
double rLimitHigh = 1000.; //Maximum distance before photon is considered 'escaped to infinity'
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//The prefix for the different output files
std::string outFilePrefix;// = "./test_new.txt";///n/a3/cjtaylor/raytrace_thindisk/raytrace_data/mcmc_test_a09_i15_acc0.txt";
