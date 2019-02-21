//Phystical parameters
double a;// = 0.0;
double inclination;// = (M_PI/180.)*15.0;
double accretion;//; = 0.; //mass accretion rate

//Position of observer
double obsRadius = 1000.; //radius of observer
double obsPhi = 0.; //phi angle of observer

//Integration variables
//int n;// = 500; //the number of photons shot per side (total is n x n)
double imgSize; //length of image plane in rg
double maxStep = 100000000;  //maximal number of steps
double horizonStop = 0.01;  //minimum radial distance from event horizon before considered lost
double tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

//Output .txt file name
const char* outFileName = "./test_new.txt";///n/a3/cjtaylor/raytrace_thindisk/raytrace_data/mcmc_test_a09_i15_acc0.txt";
