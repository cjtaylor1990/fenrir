#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double rIsco(double spin) {
	double z1 = 1.0 + pow(1.0 - (spin*spin),1.0/3.0)*(pow(1.0+spin, 1.0/3.0) + (pow(1.0-spin, 1.0/3.0)));
	double z2 = sqrt((3.0*spin*spin) + z1*z1);
	double firstTerm = 3.0 + z2;
	double secondTerm = sqrt((3.0 - z1)*(3.0 + z1 + 2.0*z2));
	if (spin < 0) {
		return firstTerm + secondTerm;
	} else {
		return firstTerm - secondTerm;
	}
}

double fenrirDiskOmega(double spin, double cylRadius) {
	return 1.0/(sqrt(cylRadius*cylRadius*cylRadius) + spin);
}

double fenrirDiskZ(double spin, double thickness, double cylRadius) {
	return thickness*(1.0 - sqrt(rIsco(spin)/cylRadius));
}

double kerrDelta(double spin, double radius) {
	return (radius*radius) - (2.0*radius) + (spin*spin);
}

double kerrSigma(double spin, double radius, double theta) {
	return (radius*radius) + pow(spin*cos(theta), 2.0);
}

double kerrTT(double spin, double radius, double theta) {
	return -1.0 + (2.0*radius/kerrSigma(spin, radius, theta));
}

double kerrTPh(double spin, double radius, double theta) {
	return -2.0*spin*radius*pow(sin(theta), 2.0)/kerrSigma(spin, radius, theta);
}

double kerrPhPh(double spin, double radius, double theta) {
	double firstTerm = (radius*radius) + (spin*spin);
	double secondTerm = 2.0*spin*spin*radius*pow(sin(theta), 2.0)/kerrSigma(spin, radius, theta);
	return (firstTerm + secondTerm)*pow(sin(theta), 2.0);
}

double findTheta(double spin, double diskZ, double cylRadius) {
	if (diskZ > 0) {
		return atan(cylRadius/diskZ);
	} else {
		return M_PI/2.0;
	}
}

double findRadius(double spin, double diskZ, double cylRadius, double theta) {
	if (diskZ > 0) {
		return cylRadius/sin(theta);
	} else {
		return cylRadius;
	}
}

double fenrirGfactor(double spin, double thickness, double coronaHeight, double cylRadius) {
	double iscoVal = rIsco(spin);
	double diskZ = fenrirDiskZ(spin, thickness, cylRadius);
	double diskTheta = findTheta(spin, diskZ, cylRadius);
	double diskRadius = findRadius(spin, diskZ, cylRadius, diskTheta);
	double coronaTT = kerrTT(spin, coronaHeight, 0.0);
	double diskTT = kerrTT(spin, diskRadius, diskTheta);
	double diskTPh = kerrTPh(spin, diskRadius, diskTheta);
	double diskPhPh = kerrPhPh(spin, diskRadius, diskTheta);
	double diskOmega = fenrirDiskOmega(spin, cylRadius);

	double bottom = diskTT + (2.0*diskTPh*diskOmega) + (diskPhPh*diskOmega*diskOmega);

	return sqrt(coronaTT/bottom);

}

int main(int argc, char* argv[]) {
	double spin = atof(argv[1]);
	double coronaHeight = atof(argv[2]);
	double thickness = atof(argv[3]);
	int numberOfRadii = atoi(argv[4]);
	double rMax = atof(argv[5]);

	double rMin = rIsco(spin);
	double delR = (rMax-rMin)/(numberOfRadii-1);

	printf("%f %f\n",spin, rMin);

	double currentG;
	double currentCylRadius;
	FILE *fp;
	fp = fopen("output.txt", "w");
	for (int i = 0; i < numberOfRadii; i++) {
		currentCylRadius = rMin + (delR*i);
		currentG = fenrirGfactor(spin, thickness, coronaHeight, currentCylRadius);
		fprintf(fp, "%f %f\n", currentCylRadius, currentG);
	}
	fclose(fp);

	return 0;

}