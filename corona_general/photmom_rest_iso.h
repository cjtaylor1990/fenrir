#include <cmath>

double restE(void){
	double restEout;
	restEout = 1.;
	return restEout;
}

double restPt(void){
	double restPtOut;
	restPtOut = restE();
	return restPtOut;
}

double restPr(double alpha, double beta){
	double restPrOut;
	restPrOut = restE()*std::cos(alpha);
	return restPrOut;
}

double restPth(double alpha, double beta){
	double restPthOut;
	restPthOut = restE()*std::sin(alpha)*std::cos(beta);
	return restPthOut;
}

double restPph(double alpha, double beta){
	double restPphOut;
	restPphOut = restE()*std::sin(alpha)*std::sin(beta);
	return restPphOut;
}
