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
	const adlink = new addon.MyObject(10);
	//const adlink2 = new addon.MyObject(1);
	console.log(adlink.plusOne());
	// Prints: 11
	console.log(adlink.plusOne());
	// Prints: 12
	console.log(adlink.plusOne());
	//console.log(adlink2.plusOne());
	console.log(adlink.link());
	//var di=adlink.getD()
	//console.log("-")
	//console.log(di);
	//adlink.openVLV(0);
	console.log(adlink.getDO())
	adlink.closeVLV(0);
	console.log(adlink.getDI())
	console.log(adlink.getDIBit(0));
	console.log(adlink.getDIBit(2));
	console.log(adlink.getDIBit(4));
	console.log(adlink.getDIBit(6));
	console.log(adlink.getDIBit(8)==false);
	// console.log(adlink.getchanelVolt(3));
	// adlink.putchanelVolt(3,-10);
	// console.log(adlink.getchanelVolt(3));
}
function testatexit(){
	var addon = require('bindings')('hello');
}
//testadd();
//testcallback();
//testobj();
//testfunction();
testWrapObj();
//testatexit();
// Public Enum vlvindex
//     enmZongyang = 0
//     enmGaoPin = 1
//     enmPanglu = 2
//     enmLushi = 3
//     enmYangqiang = 4
//     enmLuhou = 5
//     enmpaikong = 6
//     enmShengJiang = 7
//     enmShuaLU = 8
//     enmGpPower = 9
//     enmXichen = 10
//     enmHuancui = 11
//     enmLLTJ = 12
// End Enum
// Public Enum diIndex
//     enmZongyang = 0
//     enmDongli = 2
//     enmLushua = 6
//     enmLuhou = 4
//     enmShenglu = 8
    
// End Enum