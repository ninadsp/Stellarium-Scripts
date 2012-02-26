//
// Stellarium Scripts
//
// http://github.com/ninadsp/Stellarium-Scripts
//
// Common functions for scripts
//

// Stellarium Script Helper - The object which includes all
// functions and variables
var ssh = ssh || function() {
	// Default settings
	defaults: {},
	// Initialize the night sky according to defaults and 
	// values passed by the user
	initEnv: function() {},
	// Point out a series of objects in the sky
	pointObjects: function(objectArray) {},
	// Display text on screen
	commentary: function() {},
	// Wrapper over LabelMgr.labelObject
	labelObject: function() {},
	// wait till timestamp
	waitTill: function(timestamp) {}
};
