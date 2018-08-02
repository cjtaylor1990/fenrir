#include <cmath>

double dblRandAlpha(){
	double randAlphaOut = (double)rand()/RAND_MAX;
	return -1. + (2.*randAlphaOut);
}
double dblRandBeta(){
	double randBetaOut = (double)rand()/RAND_MAX;
	return randBetaOut;
}
