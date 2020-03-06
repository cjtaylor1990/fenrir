#include <cmath>
#include "metric.hpp"
#include "disk.hpp"

double cylindricalR(double position[4]) {
	return position[1]*std::sin(position[2]);
}

double verticalHeight(double position[4]) {
	return position[1]*std::cos(position[2]);
}

Disk::Disk(Metric metric, double newDiskThickness) {
	double a = metric.spin();
	double z1 = 1. + (pow(1-a*a,(1./3.))*((pow(1+a,(1./3.)))+(pow(1-a,(1./3.))))); //
	double z2 = sqrt((3.*a*a)+(z1*z1)); //
	if (a < 0.){
		rIsco = 3.+z2+sqrt((3-z1)*(3+z1+(2.*z2))); //
	}else{
		rIsco = 3.+z2-sqrt((3-z1)*(3+z1+(2.*z2)));
	}
	diskThickness = newDiskThickness;
}

double Disk::scaleHeight(double position[4]) {
	double r = cylindricalR(position);
	if (r < rIsco){
		return 0.;
	}
	else{
		return diskThickness*(1 - sqrt(rIsco/r));
  	}
}

double Disk::diskPhiVel(Metric metric, double position[4]) {
	double a = metric.spin();
	double r = cylindricalR(position);
	return 1./(sqrt(r*r*r)+(a));
}

double Disk::diskTdot(Metric metric, double position[4]){
	double diskPhiVelValue = diskPhiVel(metric, position);
	double gTTvalue = metric.gTT(position);
	double gTPhValue = metric.gTPh(position);
	double gPhPhValue = metric.gPhPh(position);
	double diskTdotSqValue = -1.*(gTTvalue+(2.*gTPhValue*diskPhiVelValue)+(gPhPhValue*diskPhiVelValue*diskPhiVelValue));
	return sqrt(1./diskTdotSqValue);
}

double Disk::diskPhiDot(Metric metric, double position[4]){
	double diskPhiVelValue = diskPhiVel(metric, position);
	double diskTdotValue = diskTdot(metric, position);
	return diskTdotValue*diskPhiVelValue;
}

double Disk::diskThDot(Metric metric, double position[4]){
	return 0.;
}

double Disk::diskRdot(Metric metric, double position[4]){
	return 0.;
}

