#include <cmath>
#include "metric.hpp"

#ifndef DISK_H // include guard
#define DISK_H

class Disk {

	double diskThickness, rIsco;

	public:

		Disk(Metric metric, double newDiskThickness);

		double scaleHeight(double position[4]);

		bool isAbove(double position);

		double diskPhiVel(Metric metric, double position[4]);

		double diskTdot(Metric metric, double position[4]);

		double diskPhiDot(Metric metric, double position[4]);

		double diskThDot(Metric metric, double position[4]);

		double diskRdot(Metric metric, double position[4]);

};

double cylindricalR(double position[4]);

double verticalHeight(double position[4]);

#endif //PHOTON_H