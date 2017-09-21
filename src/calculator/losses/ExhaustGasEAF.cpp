/**
 * @file
 * @brief Contains the implementation of the Exhaust Gas EAF loss calculator.
 *
 * @author Autumn Ferree (ferreeak)
 * @bug No known bugs.
 *
 */
#include "calculator/losses/ExhaustGasEAF.h"

double ExhaustGasEAF::getTotalHeatExhaust() {
    const double COheatingValue = CO_ * 321 / 100;
	const double H2heatingValue = H2_ * 325 / 100;
    const double combustibles = combustibleGases_ * 1020 / 100;
    const double chemicalHeat = COheatingValue + H2heatingValue + combustibles;

    const double sensibleHeat = 0.0225 * (offGasTemp_ - 60);
	const double heatInDust = dustLoading_ * 0.25 * (offGasTemp_ - 60);
	const double totalHeatFlue = chemicalHeat + sensibleHeat + heatInDust;
    const double totalVfr = vfr_ * 520 / (460 + offGasTemp_);
    return totalVfr * totalHeatFlue * 60;
}
