#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "interpol_relxill.h"

double test_func(double a1, double a2, double a3){
	return 0.5*a1 + 0.8*a2 + 0.1*a3;
}

int main(void){
	//Defining variables that will hold sample point information
	double xSample[2];
	double ySample[2];
	double zSample[2];
	
	//Defining position vector that will be point that we want to evaluate at
	double interpolPos[3];
	
	xSample[0] = 0.;
	xSample[1] = 10.;
	ySample[0] = 0.;
	ySample[1] = 10.;
	zSample[0] = 0.;
	zSample[1] = 10.;
	
	interpolPos[0] = 4.;
	interpolPos[1] = 8.;
	interpolPos[2] = 5.;
	
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
	std::cout << "ifrac1: " << ifrac1 << "\n";
	std::cout << "ifrac2: " << ifrac2 << "\n";
	std::cout << "ifrac3: " << ifrac3 << "\n";
	std::cout << "r111: " << r111 << "\n";
	std::cout << "r211: " << r211 << "\n";
	std::cout << "r121: " << r121 << "\n";
	std::cout << "r112: " << r112 << "\n";
	std::cout << "r221: " << r221 << "\n";
	std::cout << "r122: " << r122 << "\n";
	std::cout << "r212: " << r212 << "\n";
	std::cout << "r222: " << r222 << "\n";
	
	return 0;
	
}