/**
 * @file
 * @brief Function prototypes for the PSAT result fields
 *
 * This contains the prototypes for the PSAT results structure
 * including getters and setters for the important fields. Primary
 * importance are methods for calculating the existing and optimal results.
 *
 * @author Subhankar Mishra (mishras)
 * @author Gina Accawi (accawigk)
 * @bug No known bugs.
 *
 */

#ifndef AMO_LIBRARY_RESULTS_H
#define AMO_LIBRARY_RESULTS_H

#include "Motor.h"
#include "psat/Pump.h"
#include "Financial.h"
#include "FieldData.h"

/**
 * PSAT Result class
 * Contains all of the final results of the pump calculations.
 * Used to calculate the existing efficiency of the pump and the optimal efficiency of the pump so they can be compared.
 */
class PSATResult {

public:
    PSATResult() = default;

    /**
     * Constructor
     * @param pump Pump, contains all pump-related calculations, passed by reference
     * @param motor Motor, contains all motor-related calculations, passed by reference
     * @param financial Financial, contains all financial-related calculations, passed by reference
     * @param fieldData FiledData, contains all field data-related calculations, passed by reference
     * @return nothing
     */
    PSATResult(
        Pump &pump,
        Motor &motor,
        Financial &financial,
        FieldData &fieldData
    ) :
        pump_(pump),
        motor_(motor),
        financial_(financial),
        fieldData_(fieldData)
    {
        baselinePumpEfficiency_ = 0.0;
    };

    /**
     * Constructor
     * @param pump Pump, contains all pump-related calculations, passed by reference
     * @param motor Motor, contains all motor-related calculations, passed by reference
     * @param financial Financial, contains all financial-related calculations, passed by reference
     * @param fieldData FiledData, contains all field data-related calculations, passed by reference
     * @return nothing
     */
    PSATResult(
            Pump &pump,
            Motor &motor,
            Financial &financial,
            FieldData &fieldData,
            double baselinePumpEfficiency
    ) :
            pump_(pump),
            motor_(motor),
            financial_(financial),
            fieldData_(fieldData),
            baselinePumpEfficiency_(baselinePumpEfficiency)
    {};

    /**
     * A structure
     * Result structure captures the same fields for the existing as well as the optimal condition.
     */

    struct result_ {
        double pumpEfficiency_;     ///< Existing: Existing pump efficiency is fluid power added by the pump divided by pump input shaft power.
        ///< Optimal: Optimal pump efficiency is estimated based on the efficiency estimating algorithms contained in Hydraulic Institute Standard HI 1.3-2000, Centrifugal Pump Design and Application.
        double motorRatedPower_;    ///< Existing: Existing motor nameplate power (same as Rated power in the Motor input section).
        ///< Optimal: This is the nameplate motor rated power for an optimally sized pump.
        double motorShaftPower_;    ///< Existing: This is the estimated motor shaft power for the existing motor. The estimate is based on measured electrical data and PSAT's efficiency estimate for the specified motor size, speed, and class.
        ///< Optimal: This is the motor shaft power requirements for the optimal pump, based on the specified flow rate, head, and specific gravity values, along with the HI 1.3 achievable efficiency algorithms. If a belt drive is specified, associated losses are added to the pump shaft power to determine required motor power. For direct-driven pumps, the pump and motor shaft powers are the same.
        double pumpShaftPower_;     ///< Existing: This is the estimated pump shaft power for the existing motor. The estimate is the same as the motor shaft power (above) for direct-driven applications. For belt-driven applications, belt losses are deducted from the motor shaft power to determine pump shaft power.
        ///< Optimal: This is the shaft power requirements for the optimal pump, based on the specified flow rate, head, and specific gravity values, along with the HI 1.3 achievable efficiency algorithms.
        double motorEfficiency_;    ///< Existing: This is the estimated efficiency of the existing motor at the existing load.
        ///< Optimal: This is the estimated efficiency for an energy-efficient motor of the size indicated in the optimal motor rated power entry above when operating at the optimal motor shaft power.
        double motorPowerFactor_;   ///< Existing: This is the estimated power factor for the existing motor at the existing load. It is based on the measured electrical data and the motor performance characteristic curves for the specified motor.
        ///< Optimal: This is the estimated power factor for an energy-efficient motor of the size indicated in the optimal motor rated power entry above when operating at the optimal motor shaft power (also indicated above).
        double motorCurrent_;       ///< Existing: This is the estimated or measured current for the existing motor at the existing load.
        ///< Optimal: This is the estimated current for an energy-efficient motor of the size indicated in the optimal motor rated power entry above when operating at the optimal motor shaft power.
        double motorPower_;         ///< Existing: This is the estimated or measured electric power for the existing motor at the existing load.
        ///< Optimal: The estimated electric power for an energy-efficient motor of the size indicated in the optimal motor rated power entry above when operating at the optimal motor shaft power.
        double annualEnergy_;       ///< Existing: This is the annual energy consumption at the measured/estimated power level for the existing equipment when operated for the fraction of time indicated by the operating fraction.
        ///< Optimal: The annual energy consumption for an optimized pump driven by an energy-efficient motor, based on the estimated motor power and on the fraction of time the pump is operated indicated by the operating fraction.
        double annualCost_;         ///< Existing: This is the existing annual energy cost based on the product of the existing annual energy consumption and the unit operating cost (cents/kwhr) input.
        ///< Optimal: This is the annual energy cost based on the product of the optimal annual energy consumption and the unit operating cost (cents/kwhr) input.
        double estimatedFLA_;       ///< Existing: The full load amps are either specified (known) or estimated. This field will hold either one. The estimated full load amps are calcluated from the motor voltage, hp, and speed.
    };

    /**
     *
     * Gets the annual savings potential
     *
     * @return double, annual savings potential in $/year
     */
    double getAnnualSavingsPotential() const {
        return annualSavingsPotential_;
    }

    /**
     *
     * Gets the optimization rating
     *
     * @return double, optimization rating as %
     */
    double getOptimizationRating() const {
        return optimizationRating_;
    }

    /**
     *
     * Gets the existing conditions
     *
     * @return const result_, existing conditions
     */
    const result_ &getExisting() const {
        return existing_;
    }

    /**
     *
     * Gets the modified conditions
     *
     * @return const result_, modified conditions
     */
    const result_ &getModified() const {
        return modified_;
    }

    /**
     *
     * Gets the optimal conditions
     *
     * @return const result_, optimal conditions
     */
    const result_ &getOptimal() const {
        return optimal_;
    }

    /**
     * Calculates existing conditions
     * @return double, existing conditions
     */
    double calculateExisting();

    /**
     * Calculates modified conditions
     * @return double, modified conditions
     */
    double calculateModified();

    /**
     * Calculates optimal conditions
     * @return double, optimal conditions
     */
    double calculateOptimal();



private:
    // Out values
    result_ existing_, optimal_, modified_;
    double annualSavingsPotential_ = 0.0;
    double optimizationRating_ = 0.0;
    // In values
    Pump pump_;
    Motor motor_;
    Financial financial_;
    FieldData fieldData_;
    double baselinePumpEfficiency_;
};


#endif //AMO_LIBRARY_RESULTS_H
