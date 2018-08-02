#include <cmath>

double pT(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[0]*eTvec[0]) + (restMomVec[3]*ePhVec[0]);
}
	
double pR(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[1]*eRvec[1]);
}

double pTh(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[2]*eThVec[2]);
}

double pPh(double restMomVec[4],double eTvec[4], double eRvec[4], double eThVec[4], double ePhVec[4]){
	return (restMomVec[0]*eTvec[3]) + (restMomVec[3]*ePhVec[3]);
}
