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
	defaults: {
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
		constellationArt: false
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
			dist: "20"
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
	initEnv: function(parameters) {

		// Merge defaults and parameters passed by user
		// and store in ssh.config
		ssh.config = merge(defaults, parameters);

		// Delete all previous labels
		LabelMgr.deleteAllLabels();

		// Clear the environment, set location and time, and other properties

		core.clear(ssh.config.state);
		core.setProjectionMode(ssh.config.projectionMode);
		core.setObserverLocation(ssh.config.lat, ssh.config.lon, ssh.config.alt,1,"",ssh.config.planet);
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
	pointObjects: function(objectArray, wait) {
		if(isNaN(wait)) {
			wait = ssh.config.pointerWait;
		}

		for(var x in array)	{
			core.selectObjectByName(array[x], true);
			core.wait(wait);
		}
	},
	// Display text on screen
	commentary: function(parameters) {},
	// Wrapper over LabelMgr.labelObject
	labelObject: function(parameters) {},
	// wait till timestamp
	waitTill: function(timestamp) {},
	// merge two arrays
	merge: function(a, b) {
		var merged = a.clone();
		for(var property in b) {
			if( b.hasOwnProperty(property) ) {
				merged[property] = b[property];
			}
		}
		return merged;
	},
	// debug
	debug: function(string) {
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
