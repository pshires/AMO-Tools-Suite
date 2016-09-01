//
// Created by Accawi, Gina K. on 6/17/16.
//

#ifndef AMO_LIBRARY_RESULTS_H
#define AMO_LIBRARY_RESULTS_H

class PSATResult {

public:
    PSATResult() = default;

    struct result_ {
        double pumpEfficiency_;
        double motorRatedPower_;
        double motorShaftPower_;
        double pumpShaftPower_;
        double motorEfficiency_;
        double motorPowerFactor_;
        double motorCurrent_;
        double motorPower_;
        double annualEnergy_;
        double annualCost_;
    };


    int calculate();

    double getAnnualSavingsPotential() const {
        return annualSavingsPotential_;
    }

    double getOptimizationRating() const {
        return optimizationRating_;
    }


    const result_ &getExisting() const {
        return existing_;
    }

    const result_ &getOptimal() const {
        return optimal_;
    }

private:
    result_ existing_, optimal_;
    double annualSavingsPotential_;
    double optimizationRating_;
};


#endif //AMO_LIBRARY_RESULTS_H