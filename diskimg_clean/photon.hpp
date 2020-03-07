#include <cmath>
#include "metric.hpp"

#ifndef PHOTON_H // include guard
#define PHOTON_H

class Photon {
	double position[4];

	double momentum[4];

	double energy, angMom, carter;

	double rSwitch, thetaSwitch;

	public:
		Photon(double newPosition[4], double newEnergy, double newAngMom, double newCarter, double newRswitch, double newThetaSwitch);

		double time();

		double radius();

		double theta();

		double phi();

		double getRswitch();

		double getThetaSwitch();
		
		void getMomentum(double vector[4]);

		void updatePosition(double newPosition[4]);

		void updateMomentum(double newMomentum[4]);

		void updateRswitch(double newRswitch);

		void updateThetaSwitch(double newThetaSwitch);
		
		double tdot(Metric metric, double testPosition[4]);

		double phidot(Metric metric, double testPosition[4]);

		double thdotsq(Metric metric, double testPosition[4]);

		double rdotsq(Metric metric, double testPosition[4]);
};

#endif //PHOTON_H