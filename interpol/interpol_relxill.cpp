#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "interpol_relxill.h"

double test_func(double a1, double a2, double a3){
	return a1 + 2.*a2 + 5.*a3;
}

int main(void){
	//Defining variables that will hold sample point information
	double xSample[2];
	double ySample[2];
	double zSample[2];
	
	//Defining position vector that will be point that we want to evaluate at
	double interpolPos[3];
	
	xSample[0] = 0.;
	xSample[1] = 1.;
	ySample[0] = 0.;
	ySample[1] = 1.;
	zSample[0] = 0.;
	zSample[1] = 1.;
	
	interpolPos[0] = 0.5;
	interpolPos[1] = 0.5;
	interpolPos[2] = 0.5;
	
	//Evaluating the function at sample points
	double r111,r211,r121,r221,r112,r212,r122,r222;
	r111 = test_func(xSample[0],ySample[0],zSample[0]);
	r211 = test_func(xSample[1],ySample[0],zSample[0]);
	r121 = test_func(xSample[0],ySample[1],zSample[0]);
	r221 = test_func(xSample[1],ySample[1],zSample[0]);
	r112 = test_func(xSample[0],ySample[0],zSample[1]);
	r212 = test_func(xSample[1],ySample[0],zSample[1]);
	r122 = test_func(xSample[0],ySample[1],zSample[1]);
	r222 = test_func(xSample[1],ySample[1],zSample[1]);
	
	//Creating the fractional step size in three dimensions
	double ifrac1,ifrac2,ifrac3;
	ifrac1 = (interpolPos[0]-xSample[0])/(xSample[1]-xSample[0]);
	ifrac2 = (interpolPos[1]-ySample[0])/(ySample[1]-ySample[0]);
	ifrac3 = (interpolPos[2]-zSample[0])/(zSample[1]-zSample[0]);
	
	//double interp_lin_3d(double ifrac1, double ifrac2, double ifrac3, double r111, double r211, double r121, double r112, double r221, double r122, double r212, double r222)
	double interpol = interp_lin_3d(ifrac1,ifrac2,ifrac3,r111,r211,r121,r112,r221,r122,r212,r222);
	double exact = test_func(interpolPos[0],interpolPos[1],interpolPos[2]);
	std::cout << "Interpolated Result: " << interpol << "\n";
	std::cout << "Exact Result: " << exact << "\n";
	std::cout << "Relative Error: " << ((interpol-exact)/exact)*100. << " % \n";
	
	return 0;
	
}