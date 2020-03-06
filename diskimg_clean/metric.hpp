#include <cmath>

#ifndef METRIC_H // include guard
#define METRIC_H

class Metric {
    double a, rEvent;

    public:
        Metric(double spin);

        double spin();

		double eventHorizon();

        double sigma(double position[4]);

        double delta(double position[4]);
    
        double gTT(double position[4]);

        double gTPh(double position[4]);

        double gPhT(double position[4]);

        double gRR(double position[4]);

        double gThTh(double position[4]);

        double gPhPh(double position[4]);
};
#endif //METRIC_H
