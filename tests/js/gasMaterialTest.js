/**
 * Created by g9a on 3/31/17.
 */
const test = require('tap').test
, testRoot = require('path').resolve(__dirname, '../../')
, bindings = require('bindings')({ module_root: testRoot, bindings: 'phast'});

test('phastGaseMaterial', function (t) {
    t.plan(1);
    t.type(bindings.selectSolidMaterial, 'function');


});