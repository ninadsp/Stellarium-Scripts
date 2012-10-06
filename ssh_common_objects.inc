//
// Name: Stellarium Scripts - Common Objects
// License: Public Domain
// Author: Ninad Pundalik
// Description: A catalog of objects which are commonly shown in a session
// http://github.com/ninadsp/Stellarium-Scripts
//

// Stellarium Script Helper - The object which includes all
// functions and variables
var ssh = ssh || {};

// Objects container
//
// Each object will consist of a object identifier (id) and 
// an english name (engName) attribute
//
ssh.objects = {};

// Array of planets
ssh.objects.planets = {};

// Array of Messier objects
ssh.objects.M = {};

// Array of constellations
ssh.objects.constellations = {};

// Array of Nebulae
ssh.objects.nebulae = {};

// Array of galaxies
ssh.objects.galaxies = {};

// Array of other objects
ssh.objects.others = {};

ssh.objects.others.summerTriangle = [
		{ "id": "HIP 97649", "engName": "Altair" },
		{ "id": "HIP 102098", "engName": "Deneb" },
		{ "id": "HIP 91262", "engName": "Vega" }
	];
ssh.objects.others.winterTriangle = [
		{ "id": "HIP 32349", "engName": "Sirius" },
		{ "id": "HIP 27989", "engName": "Betelgeuse" },
		{ "id": "HIP 37379", "engName": "Procyon" }
	];
