#ifndef AMO_TOOLS_SUITE_GASFLUEGASMATERIAL_H
#define AMO_TOOLS_SUITE_GASFLUEGASMATERIAL_H

class GasCompositions;

std::vector<GasCompositions> SQLite::get_default_gas_flue_gas_materials() {
    return {
		    {"Natural Gas Pennsylvania", 83.4, 15.8, 0.8, 0, 0, 0, 0, 0, 0, 0, 0},
		    {"Natural Gas South Carolina", 84.0, 14.8, 0.5, 0, 0, 0, 0, 0, 0.7, 0, 0},
		    {"Natural Gas Ohio", 93.33, 0.25, 3.4, 1.82, 0, 0, 0, 0.45, 0.22, 0, 0.35},
		    {"Natural Gas Louisiana", 90.0, 5.0, 5.0, 0, 0, 0, 0, 0, 0, 0, 0},
		    {"Natural Gas Oklahoma", 84.1, 6.7, 8.4, 0, 0, 0, 0, 0, 0.8, 0, 0},
		    {"Coke Oven Gas", 33.9, 5.2, 3.7, 47.9, 0, 0, 0, 6.1, 2.6, 0, 0.6},
		    {"Blast Furnace Gas", 0.1, 0, 56.4, 2.4, 0, 0, 3.4, 23.3, 14.4, 0, 0},
    };
}

#endif //AMO_TOOLS_SUITE_GASFLUEGASMATERIAL_H
