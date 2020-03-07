#include "metric.hpp"

#ifndef OBSERVER_H // include guard
#define OBSERVER_H

class Observer {

    int imgSize;
    int photonNumber;

    double initPosition[4];

    public:

        Observer(double newInitPosition[4], int newImgPlane, int newPhotonNumber);

        double imgX(int xIndex);
	    double imgY(int yIndex);

        double energy(Metric metric, int xIndex, int yIndex);
		double angMomentum(Metric metric, int xIndex, int yIndex);
		double carter(Metric metric, int xIndex, int yIndex);

        double rSwitch(int xIndex, int yIndex);
        double thetaSwitch(int xIndex, int yIndex);

};

#endif //Observer