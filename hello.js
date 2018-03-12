'use strict'
var addon = require('./build/Release/hello.node');
exports.hello = function() { return addon.hello() }
