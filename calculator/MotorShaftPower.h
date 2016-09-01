//
// Created by Accawi, Gina K. on 6/17/16.
//

#ifndef AMO_LIBRARY_MOTORSHAFTPOWER_H
#define AMO_LIBRARY_MOTORSHAFTPOWER_H

#include <cmath>
#include<iostream>
class MotorShaftPower {


public:
    enum class EfficiencyClass {
        STANDARD,
        ENERGY_EFFICIENT,
        AVERAGE,
        SPECIFIED
    };

    MotorShaftPower(double motorEfficiency, double motorPower, int motorRPM, EfficiencyClass efficiencyClass)
            : motorEfficiency_(motorEfficiency),
              motorPower_(motorPower), motorRPM_(motorRPM), efficiencyClass_(efficiencyClass) {};
    double calculate();
    double getMotorEfficiency() const {
        return motorEfficiency_;
    }

    void setMotorEfficiency(double motorEfficiency) {
        motorEfficiency_ = motorEfficiency;
    }

    double getMotorPower() const {
        return motorPower_;
    }

    void setMotorPower(double motorPower) {
        motorPower_ = motorPower;
    }

private:
    double motorEfficiency_;
    double motorPower_;
    int motorRPM_;
    EfficiencyClass efficiencyClass_;
    double motorShaftPower_;
    double measuredPower_ = 80; // Hardcoded for now.
};


#endif //AMO_LIBRARY_MOTORSHAFTPOWER_H
