#include <cmath>
#include <stdlib.h>
#include "metric.hpp"
#include "observer.hpp"

Observer::Observer(double newInitPosition[4], int newImgSize, int newPhotonNumber) {
    imgSize = newImgSize;
    photonNumber = newPhotonNumber;
            
    initPosition[0] = newInitPosition[0];
    initPosition[1] = newInitPosition[1];
    initPosition[2] = newInitPosition[2];
    initPosition[3] = newInitPosition[3];
}

double Observer::imgX(int xIndex) {
    return ((xIndex - (photonNumber/2.))+0.5)*(imgSize/photonNumber);
}

double Observer::imgY(int yIndex) {
    return ((yIndex - (photonNumber/2))+0.5)*(imgSize/photonNumber);
}

double Observer::energy(Metric metric, int xIndex, int yIndex) {
    return 1.0;
}

double Observer::angMomentum(Metric metric, int xIndex, int yIndex) {
    //calculating Euclidean 2-D distance on image plane
    double imgXvalue = imgX(xIndex);
    double imgYvalue = imgY(yIndex);
    double imgB = pow(pow(imgXvalue,2.)+pow(imgYvalue,2.),0.5);
    
    return -1.*imgB*std::sin(initPosition[2])*(imgXvalue/imgB);
}

double Observer::carter(Metric metric, int xIndex, int yIndex) {
    //calculating Euclidean 2-D distance on image plane
    double imgXvalue = imgX(xIndex);
    double imgYvalue = imgY(yIndex);
    double imgB = pow(pow(imgXvalue,2.)+pow(imgYvalue,2.),0.5);

    double a = metric.spin();
    
    double energyValue = energy(metric, xIndex, yIndex);
    double angMomValue = angMomentum(metric, xIndex, yIndex);

    return pow(imgB*(imgYvalue/imgB),2.) - energyValue*pow(a*std::cos(initPosition[2]),2.) 
        + pow(angMomValue*std::cos(initPosition[2])/std::sin(initPosition[2]), 2.);
}

double Observer::rSwitch(int xIndex, int yIndex) {
    return 1;
}

double Observer::thetaSwitch(int xIndex, int yIndex) {
    double imgYvalue = imgY(yIndex);
    if (imgYvalue < 0.){
		return -1.;
	}else{
		return 1.;
	}
}