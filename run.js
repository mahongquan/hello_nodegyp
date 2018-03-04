'use strict'
function testadd(){
	var addon = require('bindings')('hello');
	var w=addon.add(2,3);
	console.log(w);
}
function testcallback(){
	var addon = require('bindings')('hello');
	addon((msg) => {
	  console.log(msg);
	// Prints: 'hello world'
	});
}
function testobj(){
	var addon = require('bindings')('hello');
	const obj1 = addon('hello');
	const obj2 = addon('world');
	console.log(obj1.msg, obj2.msg);
}
function testfunction(){
	var addon = require('bindings')('hello');
	const fn = addon();
	console.log(fn());
}
function testWrapObj(){
	var addon = require('bindings')('hello');
	const obj = new addon.MyObject(10);
	console.log(obj.plusOne());
	// Prints: 11
	console.log(obj.plusOne());
	// Prints: 12
	console.log(obj.plusOne());
}
function testatexit(){
	var addon = require('bindings')('hello');
}
//testadd();
//testcallback();
//testobj();
//testfunction();
//testWrapObj();
testatexit();