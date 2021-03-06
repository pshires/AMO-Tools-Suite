/**
 * @file
 * @brief Contains the two main modules of PSATResult class and the FanResult class.
 *
 * Contains 2 important functions:
 *          calculateExisting : calculates the values for the existing data.
 *          calculateOptimal : calculates the values for the optimal case.
 *
 * @author Subhankar Mishra (mishras)
 * @author Gina Accawi (accawigk)
 * @bug No known bugs.
 *
 */

#include <fans/FanEnergyIndex.h>
#include "results/Results.h"
#include "calculator/pump/PumpShaftPower.h"
#include "calculator/pump/MoverEfficiency.h"
#include "calculator/util/AnnualCost.h"
#include "calculator/util/AnnualEnergy.h"
#include "calculator/pump/OptimalPumpEfficiency.h"
#include "calculator/pump/OptimalPumpShaftPower.h"
#include "calculator/motor/OptimalMotorShaftPower.h"
#include "calculator/motor/OptimalMotorPower.h"
#include "calculator/motor/OptimalMotorSize.h"

FanResult::Output FanResult::calculateExisting(Fan::FieldDataBaseline const & fanFieldData) {
    MotorShaftPower motorShaftPower(motor.motorRatedPower, fanFieldData.measuredPower, motor.motorRpm,
                                    motor.lineFrequency, motor.efficiencyClass, motor.specifiedEfficiency,
                                    motor.motorRatedVoltage, motor.fullLoadAmps, fanFieldData.measuredVoltage,
                                    fanFieldData.loadEstimationMethod, fanFieldData.measuredAmps);
    MotorShaftPower::Output const output = motorShaftPower.calculate();
    double const fanShaftPower = PumpShaftPower(output.shaftPower, fanInput.drive).calculate();
    double const fanEfficiency = MoverEfficiency(fanFieldData.flowRate, fanShaftPower, fanFieldData.inletPressure,
                                                 fanFieldData.outletPressure, fanFieldData.compressibilityFactor).calculate();
    double const annualEnergy = AnnualEnergy(output.power, operatingFraction).calculate();
    double const annualCost = AnnualCost(annualEnergy, unitCost).calculate();

    double const fanEnergyIndex = FanEnergyIndex(fanFieldData.flowRate, fanFieldData.inletPressure, fanFieldData.outletPressure,
                                                 fanInput.airDensity, fanFieldData.measuredPower).calculateEnergyIndex();

    return {motorShaftPower.calculate(), fanEfficiency, motor.motorRatedPower, fanShaftPower, annualEnergy, annualCost,
            fanEnergyIndex, output.estimatedFLA};
}

FanResult::Output FanResult::calculateModified(Fan::FieldDataModifiedAndOptimal const & fanFieldData, const double fanEfficiency,
                                               bool isOptimal) {
    double const fanShaftPower = OptimalPumpShaftPower(fanFieldData.flowRate, fanFieldData.inletPressure,
                                                       fanFieldData.outletPressure, fanFieldData.compressibilityFactor,
                                                       fanEfficiency).calculate();

    double const motorShaftPower = OptimalMotorShaftPower(fanShaftPower, fanInput.drive).calculate();

    OptimalMotorPower::Output const output = OptimalMotorPower(motor.motorRatedPower, motor.motorRpm, motor.lineFrequency,
                                                               motor.efficiencyClass, motor.specifiedEfficiency, motor.motorRatedVoltage,
                                                               fanFieldData.measuredVoltage, fanShaftPower).calculate(isOptimal);

    double const annualEnergy = AnnualEnergy(output.power, operatingFraction).calculate();
    double const annualCost = AnnualCost(annualEnergy, unitCost).calculate();

    double const fanEnergyIndex = FanEnergyIndex(fanFieldData.flowRate, fanFieldData.inletPressure, fanFieldData.outletPressure,
                                                 fanInput.airDensity, output.power).calculateEnergyIndex();

    return {fanEfficiency, motor.motorRatedPower, motorShaftPower, fanShaftPower, output.efficiency, output.powerFactor,
            output.current, output.power, annualEnergy, annualCost, fanEnergyIndex};
}

FanResult::Output FanResult::calculateOptimal(Fan::FieldDataModifiedAndOptimal const & fanFieldData,
                                              OptimalFanEfficiency::FanType const fanType) {
    const double fanEfficiency = OptimalFanEfficiency(fanType, fanInput.fanSpeed, fanFieldData.flowRate, fanFieldData.inletPressure,
                                                      fanFieldData.outletPressure, fanFieldData.compressibilityFactor).calculate();

    return calculateModified(fanFieldData, fanEfficiency, true);
}

FanResult::Output FanResult::calculateOptimal(Fan::FieldDataModifiedAndOptimal const & fanFieldData,
                                              const double userInputFanEfficiency) {
    return calculateModified(fanFieldData, userInputFanEfficiency, true);
}

PSATResult::Result & PSATResult::calculateExisting() {
    /**
     * 1a	Calculate motor shaft power from measured power, OR
     * 1b	Calculate motor shaft power from measured current, voltage
     *  This step calculates the following parameters of motor
     *      shaft power,
     *      current,
     *      power factor,
     *      efficiency and
     *      power.
     * 2a	If a belt drive is specified, calculate the pump shaft power
     * 2b	If direct drive, motor shaft power = pump shaft power
     * 3	Calculate fluid power
     * 4	Calculate pump efficiency
     * 5	Calculate annual energy and energy cost
     */
    MotorShaftPower motorShaftPower(motor.motorRatedPower, fieldData.motorPower, motor.motorRpm,
                                    motor.lineFrequency, motor.efficiencyClass, motor.specifiedEfficiency,
                                    motor.motorRatedVoltage, motor.fullLoadAmps, fieldData.voltage,
                                    fieldData.loadEstimationMethod, fieldData.motorAmps);
    auto const output = motorShaftPower.calculate();

    existing.motorShaftPower = output.shaftPower;
    existing.motorCurrent = output.current;
    existing.motorPowerFactor = output.powerFactor;
    existing.motorEfficiency = output.efficiency;
    existing.motorPower = output.power;
    existing.estimatedFLA = output.estimatedFLA;

	existing.motorRatedPower = motor.motorRatedPower;
    existing.pumpShaftPower = PumpShaftPower(existing.motorShaftPower, pumpInput.drive).calculate();
    existing.pumpEfficiency = MoverEfficiency(pumpInput.specificGravity, fieldData.flowRate, fieldData.head,
                                              existing.pumpShaftPower).calculate();
    existing.annualEnergy = AnnualEnergy(existing.motorPower, operatingFraction).calculate();
    existing.annualCost = AnnualCost(existing.annualEnergy, unitCost).calculate();
    return existing;
}

PSATResult::Result & PSATResult::calculateOptimal() {
    /**
     * Steps for calculating the optimal values:
     *  1. Calculate optimal pump efficiency, fluid power and pump shaft power
     *  2. Calculate Motor shaft power
     *      a. If a belt drive is specified, calculate the motor shaft power
     *      b. If direct drive, motor shaft power = pump shaft power
     *  3. Select motor size based on required motor shaft power
     *  4. Develop 25% interval motor performance data for EE motor of the selected size
     *  5. Do curve fitting of current from 25% to 1% intervals
     *  6. Do curve fitting of efficiency in 1% intervals
     *  7. Using current and efficiency 1% interval data, calculate balance of motor data in 1% intervals
     *  8. Calculate required power, motor eff., current, pf from shaft power
     *  9. Calculate annual energy and energy cost
     *  10.Calculate annual savings potential and optimization rating
     */

    OptimalPumpEfficiency optimalPumpEfficiency(pumpInput.style, pumpInput.achievableEfficiency, pumpInput.rpm,
                                                pumpInput.kviscosity, pumpInput.stageCount, fieldData.flowRate,
                                                fieldData.head);
    optimal.pumpEfficiency = optimalPumpEfficiency.calculate();
    OptimalPumpShaftPower optimalPumpShaftPower(fieldData.flowRate, fieldData.head, pumpInput.specificGravity,
                                                optimal.pumpEfficiency);
    optimal.pumpShaftPower = optimalPumpShaftPower.calculate();
    OptimalMotorShaftPower optimalMotorShaftPower(optimal.pumpShaftPower, pumpInput.drive);
    optimal.motorShaftPower = optimalMotorShaftPower.calculate();
    OptimalMotorSize optimalMotorSize(optimal.motorShaftPower, motor.sizeMargin);
    optimal.motorRatedPower = optimalMotorSize.calculate();
    OptimalMotorPower optimalMotorPower(optimal.motorRatedPower, motor.motorRpm, motor.lineFrequency,
                                        motor.efficiencyClass, motor.specifiedEfficiency,
                                        motor.motorRatedVoltage, fieldData.voltage, optimal.motorShaftPower);
    OptimalMotorPower::Output output = optimalMotorPower.calculate(true);
    optimal.motorCurrent = output.current;
    optimal.motorEfficiency = output.efficiency;
    optimal.motorPower = output.power;
    optimal.motorPowerFactor = output.powerFactor;
    // Calculate Annual Energy
    AnnualEnergy annualEnergy(optimal.motorPower, operatingFraction);
    optimal.annualEnergy = annualEnergy.calculate();

    // Calculate Annual Cost
    AnnualCost annualCost(optimal.annualEnergy, unitCost);
    optimal.annualCost = annualCost.calculate();

    // Annual Savings potential
//    annualSavingsPotential = existing.annualCost - optimal.annualCost;
    // Optimization Rating
//    optimizationRating = optimal.motorPower / existing.motorPower;
	return optimal;
}

PSATResult::Result & PSATResult::calculateModified() {
    /**
         * Steps for calculating the modified values:
     *  1. Calculate fluid power and pump shaft power
     *  2. Calculate Motor shaft power
     *      a. If a belt drive is specified, calculate the motor shaft power
     *      b. If direct drive, motor shaft power = pump shaft power
     *  4. Develop 25% interval motor performance data for specified efficiency rating motor of the selected size
     *  5. Do curve fitting of current from 25% to 1% intervals
     *  6. Do curve fitting of efficiency in 1% intervals
     *  7. Using current and efficiency 1% interval data, calculate balance of motor data in 1% intervals
     *  8. Calculate required power, motor eff., current, pf from shaft power
     *  9. Calculate annual energy and energy cost
     *  10.Calculate annual savings potential and optimization rating

     */

    if (pumpInput.style == Pump::Style::SPECIFIED_OPTIMAL_EFFICIENCY) {
        modified.pumpEfficiency = pumpInput.achievableEfficiency;
    } else {
        modified.pumpEfficiency = baselinePumpEfficiency;
    }

    OptimalPumpShaftPower modifiedPumpShaftPower(fieldData.flowRate, fieldData.head, pumpInput.specificGravity,
                                                modified.pumpEfficiency);
    modified.pumpShaftPower = modifiedPumpShaftPower.calculate();

    OptimalMotorShaftPower modifiedMotorShaftPower(modified.pumpShaftPower, pumpInput.drive);
    modified.motorShaftPower = modifiedMotorShaftPower.calculate();

    modified.motorRatedPower = motor.motorRatedPower;
    OptimalMotorPower modifiedMotorPower(modified.motorRatedPower, motor.motorRpm, motor.lineFrequency,
                                         motor.efficiencyClass, motor.specifiedEfficiency,
                                         motor.motorRatedVoltage, fieldData.voltage, modified.motorShaftPower);
    OptimalMotorPower::Output output = modifiedMotorPower.calculate(false);
    modified.motorCurrent = output.current;
    modified.motorEfficiency = output.efficiency;
    modified.motorPower = output.power;
    modified.motorPowerFactor = output.powerFactor;

    // Calculate Annual Energy
    AnnualEnergy annualEnergy(modified.motorPower, operatingFraction);
    modified.annualEnergy = annualEnergy.calculate();

    // Calculate Annual Cost
    AnnualCost annualCost(modified.annualEnergy, unitCost);
    modified.annualCost = annualCost.calculate();

    // Annual Savings potential
    //annualSavingsPotential = existing.annualCost - modified.annualCost;
    annualSavingsPotential = 0.0;
    // Optimization Rating
    //optimizationRating = modified.motorPower / existing.motorPower;
    optimizationRating = 0.0;
	return modified;
}
