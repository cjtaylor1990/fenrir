#include <cmath>

#ifndef METRIC_H // include guard
#define METRIC_H

class Metric {
    double a;

    public:
        Metric(double spin);

        double getSpin();

        double sigma(double r, double phi, double theta, double time);

        double delta(double r, double phi, double theta, double time);
    
        double gTT(double r, double phi, double theta, double time);

        double gTPh(double r, double phi, double theta, double time);

        double gPhT(double r, double phi, double theta, double time);

        double gRR(double r, double phi, double theta, double time);

        double gThTh(double r, double phi, double theta, double time);

        double gPhPh(double r, double phi, double theta, double time);
};
#endif //METRIC_H
