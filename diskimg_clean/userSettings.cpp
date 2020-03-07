#include "userSettings.hpp"

using namespace userSettings;

double userSettings::initTime = 0.;
double userSettings::initRadius = 1000.;
double userSettings::initPhi = 0.;

int userSettings::maxStepNumber = 100000000;  //maximal number of steps
double userSettings::horizonStop = 0.01;  //minimum radial distance from event horizon before considered lost
double userSettings::tolerance = 1000.; //Tolerance for automatic step refinement. The larger, the more finely sampled.

