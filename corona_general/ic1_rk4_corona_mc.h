#include <cmath>

int n = 100000.;
double a = 0.9;
double accretion = 0.0;
std::string coronaType = "lp"; //can be equal to 'lp' (lamppost) or 'offaxis' (offaxis corona)

//Lamppost variables. Need if coronaType = 'lp'
double height = 12.;  //lamppost corona height

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
double rLimitHigh = 5000.; //Maximum distance before photon is considered 'escaped to infinity'
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//Output .txt file name
const char* outFileName = "./test.txt"; //Path to .txt file to be written out to
