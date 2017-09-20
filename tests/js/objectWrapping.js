// const addon = require('../../build/Release/exhaustGasEAF');
const addon = require('../../build/Release/exhaustGasEAF');


// const test = require('tap').test
//     , testRoot = require('path').resolve(__dirname, '../../')
//     , bindings = require('bindings')({ module_root: testRoot, bindings: 'exhaustGasEAF'});


// test('exhaustGasEAF object wrap', function (t) {
var inp = {
    offGasTemp: 2800, CO: 12, H2: 10, combustibleGases: 3, vfr: 8000, dustLoading: 0.001
};

var obj = new addon.ExhaustGasEAF(2800, 12, 10, 3, 8000, 0.001);
var res = obj.getTotalHeatExhaustNAN();
console.log(res + " should be 12553119.02");


obj = new addon.ExhaustGasEAF(2800, 8, 6, 3, 6500, 0.001);
res = obj.getTotalHeatExhaustNAN();
console.log(res + " should be 8591939.26");

// });