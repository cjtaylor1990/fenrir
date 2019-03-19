#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "trilinear_interpol.h"

double test_func(double a1, double a2, double a3){
	return a1 + 2.*a2 + (a3*a3);
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
	
	double interpol = trilinear_interpol(xSample,ySample,zSample,test_func,interpolPos);
	double exact = test_func(interpolPos[0],interpolPos[1],interpolPos[2]);
	std::cout << "Interpolated Result: " << trilinear_interpol(xSample,ySample,zSample,test_func,interpolPos) << "\n";
	std::cout << "Exact Result: " << test_func(interpolPos[0],interpolPos[1],interpolPos[2]) << "\n";
	std::cout << "Relative Error: " << ((interpol-exact)/exact)*100. << " % \n";
	
	return 0;
	
}