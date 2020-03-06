#include <cmath>
#include "metric.hpp"

#ifndef PHOTON_H // include guard
#define PHOTON_H

class Photon {
	double r, phi, theta, time;

	double energy, angMom, carter;

	public:
		Photon(double newR, double newPhi, double newTheta, double newTime, double newEnergy, double newAngMom, double newCarter);

		double getRadius();
		
		double getPhi();

		double getTheta();

		double getTime();

		void updatePosition(double newR, double newPhi, double newTheta, double newTime);
		
		double tdot(Metric metric);

		double phidot(Metric metric);

		double thdotsq(Metric metric);

		double rdotsq(Metric metric);

};

#endif //METRIC_H