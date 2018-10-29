#pragma once
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
class car
{
public:
	car();
	~car();


	//Boolean variables for while loops

	bool hasFinished; //boolean variable to track if the current car in the simulation has crashed, run out of fuel, or finished

							  //tracking/index variables

	int currentCar; //int variable to track the number of the current car running in the simulation
	int numCars; //int variable to track the number of cars that have been run through the simulation
	int currentFuelStation; //int variable to act as an index for the current fuel station
	int numMoves; //int variable to keep track of how many successful moves the current car has made in the simulation
	
	//car data variables

	int carXPos; //int to track the current x position of the current car in the simulation
	int carYPos; //int to track the current y position of the current car in the simulation
	int currentFuel; //int to track how much fuel the current car in the simulation has left
	int fuelIndicator;
	int testX;
	int testY;
	int id;
	int firstFuelX;
	int firstFuelY;

	//maps
	int mapArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map data
	int mapMasterArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map that contains the data for fuel spawns and other stuff; a teacher's version essentially 
	int fuelMapArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map that contain all locations for fuel locations, w/ their values being their numbers in the sequences
	struct Coord
	{
		float x;
		float y;
	};

	int lastMove;

	std::map<int, Coord*> fuelLocations;
	std::map<int, Coord*> hazardLocations;
	std::map<int, Coord*>::iterator mapIterator;



	//Functions

	bool Initialize(); //function to initialize values
	int runSimulation(); //function to run the cars through the simualtion
	bool updateMap(); //function to update the map w/in the car's vision range
	bool findMove(); //function to find and execute the car's next move
	bool checkCar(); //function to check for various effects/condiions after the car finishes a move

	int findDistance(Coord*, Coord*);
	int getNumCars();
	;
};

