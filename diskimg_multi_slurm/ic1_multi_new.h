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
