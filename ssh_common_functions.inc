//
// Name: Stellarium Scripts - Common Functions
// License: Public Domain
// Author: Ninad Pundalik
// Description: A common set of functions used in Stellarium scripts
// http://github.com/ninadsp/Stellarium-Scripts
//

// Stellarium Script Helper - The object which includes all
// functions and variables
var ssh = ssh || function() {
	// Default settings
	var defaults = {
		// core.clear - Supported values: natural, starchart, deepspace
		state: "natural",
		// core.setProjectionMode - Supported values: ProjectionPerspective,
		// ProjectionEqualArea, ProjectionSterographic, ProjectionFisheye,
		// ProjectionHammer, ProjectionCylinder, ProjectionMercator,
		// ProjectionOrthographic
		projectionMode: "ProjectionPerspective",
		// Latitude, Longitude, Altitude, time
		// Currently set to Bangalore/Bengaluru, India and system time
		lat: 77.70,
		lon: 12.95,
		alt: 875,
		planet: "Earth",
		time: "now",
		timespec: "local",
		// The Light Pollution factor in the settings. Supported values: 1-9
		bortleScale: 6,
		// Field of View in degrees
		zoomTo: 70,
		// Show the cardinal points
		cardinalPoints: true,
		// Show fog
		fog: false,
		// Tracking
		tracking: false,
		// Isolate selected objects?
		isolateSelected: true,
		// Display constellation lines
		constellationLines: false,
		// Display constellation art
		constellationArt: false,
		// Amount of object information to be displayed. Supported values:
		// None, ShortIntro, AllInfo
		selectedObjectInfo: "ShortIntro",
		// The minimum and maximum FPS settings
		minFPS: 10,
		maxFPS: 50,
		// Initial Altitude/Azimuth values - East
		alt: 0,
		azi: 90,
		// Default values for object labels
		labels: {
			visible: true,
			size: 19,
			color: "#aaaaaa",
			// Supported values: N, S, E, W, NE, NW, SE, SW
			side: "W",
			labelDist: "20"
		},
		commentary: {
			x: 200,
			y: 100,
			visible: true,
			size: 25,
			color: "#aaaaaa",
			time: 7
		},
		// Amount of time to wait between two objects in pointObjects
		pointerWait: 7,
		// Log debug messages to the console?
		debug: false
	},
	// Initialize the night sky according to defaults and 
	// values passed by the user
	// lat, lon, alt, timestamp
	var initEnv = function(parameters) {

		// Merge defaults and parameters passed by user
		// and store in ssh.config
		ssh.config = ssh.merge(defaults, parameters);

		// Delete all previous labels
		LabelMgr.deleteAllLabels();
		ssh.labelList = new Array();

		// Clear the environment, set location and time, and other properties

		core.clear(ssh.config.state);
		core.setProjectionMode(ssh.config.projectionMode);
		core.setObserverLocation(ssh.config.lat, 
					ssh.config.lon, 
					ssh.config.alt,
					1,
					"",
					ssh.config.planet
				);
		core.setDate(ssh.config.time, ssh.config.timespec);
		StelSkyDrawer.setBortleScale(ssh.config.bortleScale);
		LandscapeMgr.setFlagCardinalsPoints(ssh.config.cardinalPoints);
		LandscapeMgr.setFlagFog(ssh.config.fog);
		StelMovementMgr.setFlagTracking(ssh.config.tracking);
		StelMovementMgr.zoomTo(ssh.config.zoomTo, 0);

		// Remove any previous selections of objects
		// Prevent older constellation lines/art showing up again
		ConstellationMgr.setFlagIsolateSelected(ssh.config.isolateSelected);
		core.setSelectedObjectInfo("None");
		core.selectObjectByName("HIP40793", false);
		core.selectObjectByName(" ", true);

		// Continue initializing
		ConstellationMgr.setFlagLines(ssh.config.constellationLines);
		core.setSelectedObjectInfo(ssh.config.selectedObjectInfo);
		core.setMinFPS(ssh.config.minFPS);
		core.setMaxFPS(ssh.config.maxFPS);
		
		// Point due East
		core.moveToAltAzi(ssh.config.alt, ssh.config.azi,1); 
		core.wait(1);
	},
	// Point out a series of objects in the sky
	var pointObjects = function(objectArray, wait) {
		if(isNaN(wait)) {
			wait = ssh.config.pointerWait;
		}

		for(var x in array)	{
			core.selectObjectByName(array[x], true);
			core.wait(wait);
		}
	},
	// Display text on screen
	var commentary = function(textArray, parameters) {

		var options = merge(ssh.config.commentary, parameters);
		var lineSep = options.size * 1.5;
		var y = options.y;
		var ids = new Array();

		// Create labels
		for(var i = 0; i < textArray.length; i++) {
			ids[i] = LabelMgr.labelScreen( textArray[i],
							options.x,
							y,
							options.visible,
							options.size,
							options.color
						);
			// do we really need to push them to the global list?
			//ssh.labelList.push(ids[i]);
			y += lineSep;
		}

		// Wait for display time
		core.wait(options.time);

		// delete all labels
		for(var i = 0; i < ids.length; i++) {
			LabelMgr.deleteLabel(ids[i]);
		}
	},
	// Wrapper over LabelMgr.labelObject
	var labelObject = function(obj, parameters) {
		var options = merge(ssh.config.labels, parameters);

		var currLabel = LabelMgr.labelObject(obj.engName, 
							obj.id, 
							options.visible, 
							options.size,
							options.color,
							options.side,
							options.labelDist
						);
		
		if(currLabel != -1)	{
			ssh.labelList.push(currLabel);
		}
		return currLabel;

	},
	// wait till timestamp (assumes that timestamp is greater than
	// current time stamp. Cannot yet handle running time backwards)
	// TODO: Handle a timestamp in history
	var waitTill = function(timestamp, timespec, timeRate) {
		if(typeof(timespec) === undefined) {
			timespec = ssh.config.timespec;
		}
		if(isNan(timeRate)) {
			timeRate = 1;
		}
		var now = core.getJDay();
		var currTimeRate = core.getTimeRate();
		var then = core.jdFromDateString(timestamp, timespec);
		var diff = then - now;

		if(diff > 0) {

			// Speed up if required
			// We should handle this more gracefully,
			// the way the solar eclipse script handles
			// it. TODO
			if(currTimeRate != timeRate) {
				core.setTimeRate(timeRate);
			}

			// mindlessly loop till we reach timestamp
			do{
				core.wait(1);
				diff = then - core.getJDay();
			}
			while( diff > 0 );

			// reset time rate if we had changed it
			if(currTimeRate != timeRate) {
				core.setTimeRate(currTimeRate);
			}
		}
	},
	// Show/Hide Constellation lines and art
	var displayConstelLinesArt = function(flag){
		if(flag) {
			ConstellationMgr.setFlagLines(true);
			core.wait(3);
			ConstellationMgr.setFlagArt(true);
		}
		else	{
			ConstellationMgr.setFlagArt(false);
			core.wait(2);
			ConstellationMgr.setFlagLines(false);
		}
	},
	// merge two arrays
	var merge = function(a, b) {
		var merged = a.clone();
		for(var property in b) {
			if( b.hasOwnProperty(property) ) {
				merged[property] = b[property];
			}
		}
		return merged;
	},
	// debug
	var debug = function(string) {
		if(ssh.debug) {
			core.debug(string);
		}
	}
};

// Object cloning code from http://my.opera.com/GreyWyvern/blog/show.dml/1725165
Object.prototype.clone = function() {
	var newObj = (this instanceof Array) ? [] : {};
	for (i in this) {
		if (i == 'clone') continue;
		if (this[i] && typeof this[i] == "object") {
			newObj[i] = this[i].clone();
		} else newObj[i] = this[i]
	} return newObj;
};
