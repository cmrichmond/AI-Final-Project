#include "car.h"
#include<random>
#include<string>
#include"Constants.h"
#include<iostream>
#include<fstream>
#include <cstdlib>

using namespace std;

int mapArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map data
int mapMasterArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map that contains the data for fuel spawns and other stuff; a teacher's version essentially 
int fuelMapArray[MAP_SIZE][MAP_SIZE]; //2d array to store the map that contain all locations for fuel locations, w/ their values being their numbers in the sequences


car::car()
{


}


car::~car()
{

}


bool car::Initialize()
{
	cout << "Enering car's Initialize function" << endl << endl;

	//Initialize variables

	fuelIndicator = 0;

	//hasWon = false;
	hasFinished = false;
	currentCar = 1;
	numCars = 0;
	carXPos = 0;
	carYPos = 0;
	currentFuel = CAR_MAX_FUEL;
	

	random_device rdev;
	default_random_engine e(rdev());
	uniform_int_distribution<int> d(0, MAP_SIZE - 1);
	ofstream myFile; //set up file output for testing FIXME REMOVE LATER
	myFile.open("coordinates.txt"); //set up output file for testing FIXME REMOVE LATER

	//Initialize the map arrays for their default spaces; unknown for the car map, empty for the fuel map, and empty for the reference map

	for (int i = 0; i < MAP_SIZE; i++) //initial loop to cycle through x coordinates
	{
		for (int j = 0; j < MAP_SIZE; j++) //secondary loop to handle y coordinates
		{
			mapArray[i][j] = UNKNOWN; //initialize the array slots as unknown so the car is blind at first
			fuelMapArray[i][j] = EMPTY; //initialize the array slots as empty for the purposes of the fuel map
			mapMasterArray[i][j] = EMPTY; //initialize all spaces as empty before we fill in the fuel spots
		}
	}


	//generate random fuel points FIX THIS TO BE MORE IDEAL LATER FIXME

	//FIXME Issue with fuel being duplicated will mean earlier one gets overwritten and no longer exists
	
	
	int fuelLocation = 11; //variable to simplify process of placing the proper enums for each fuel location in its reference map
	int previousX = 0;
	int previousY = 0; //variables to use to test each set of x and y coordinates for fuel against the last set to prevent consecutive duplicates; initially 0 to protect spawn point on 1st fuel spawn


	for (int a = 0; a < NUM_FUEL_STATIONS; a++)
	{
		int randX = d(e); //use binomial distribution to generate a random x position for a fuel point
		int randY = d(e); //use binomial distribution to generate a random y position for a fuel point

		while (randX > MAP_SIZE)
		{
			randX = d(e);
		}

		while (randY > MAP_SIZE)
		{
			randY = d(e);
		}

		while (randX == previousX) //make sure the x values dont match w/ the previous one
		{
			randX = d(e); //if they match, rerandomize and try again until they don't match
		}

		while (randY == previousY) //make sure the y values don't match w/ the previous one
		{
			randY = d(e); //if they match, rerandomize and try again until they don't match
		}


		myFile << randX << "   " << randY <<"  "<<a<< endl; //output to file for reference
	//	fuelMapArray[randX][randY] = fuelLocation; //update the fuel map array

		if (a == 0)
		{
			firstFuelX = randX;
			firstFuelY = randY;
		}


		previousX = randX; //update value of previousX
		previousY = randY; //update value of previousY

		//fuelLocation++; //increment fuel location so we can continue to use the correct enums
	}


	myFile.close();

	//cout << "Finished the initialize function for car" << endl;

	return true;
}

int car::getNumCars()
{
	return numCars;
}

//function to determine and then execute the car's next move
bool car::findMove() 
{

	/*
	
	The logic for the car's movement should be as follows. 

	STEP 1: LOCATE NEXT MOVE
	
		1.) Check if the car can see a Fuel Station on its map
			a.) If yes, its the target now. Move towards it in the x direction first, then the y direction
			b.) If no, then find the nearest unknown tile on the cars map and make it the target
				aa.) To find distance, first set a pointer carLocation* referencing mapArray[carXPos][carYPos]
				bb.) Define int closestUnknown w/ an absurdly high initial value, and int targetX and targetY without initial values
				cc.) Loop through mapArray and check each tile's enum value for UNKNOWN (0)
					aaa.) If value=0 (UNKNOWN), then find the distance from that tile to the pointer's location
					bbb.) Distance=( abs( carXPos-testXPos) + abs(carYPos-testYPos) )	
				dd.) If Distance<closestUnknown, set  closestUnknown=Distance, targetX and targetY to the x and y values of the unknown tile we just tested

	STEP 2: EXECUTE NEXT MOVE

		1.) Check if carXPos==targetXPos
			a.) If not, figure out if we need to be moving left or right in order to line up the x positions
				aa.) If carXPos>targetXPos, we're to the right of the target, and thus need to move the car to the left
					aaa.) Update the car's position (carXPos=carXPos-1;)
				bb.) If carXPos<targetXPos, we're to the left of the target, and thus need to move the car to the right
					bbb.) Update the car's position (carXPos=carXPos+1)
			b.) If yes, then we should need to move so that carYPos==targetYPos (since if we're on the fuel station, it shoudl be gone already and our new target will be
				either a new fuel station or the closest unknown tile. Check if we need to move up or down to reach the target's y position
				aa.) If carYPos<targetYPos, we're below the target, and thus need to move the car up
					aaa.) Update the car's position (carYPos=carYPos+1)
				bb.) If carYPos>targetYPos, we're above the target, and thus need to move the car down
					aaa.) Update the car's position (carYPos=carYPos-1)
		2.) Now that we've updated the car's position, we need to update the fuel and then call a function to check for a few things (if we landed on a fuel station and get a
			refill, if we've run out of fuel and hit game over for the current car, etc)
				aa.) Update the car's fuel (currentFuel=currentFuel-1) 
				bb.) Call the checkCar function
		3.) Update moveCounter (moveCounter++) and then return true so the function finishes

	*/
	/*
	
	ifstream fin;
	fin.open("coordinates.txt");



	*/
	int targetX, targetY; //int variables to record the x and y coordinates of the target space on the car's map

	bool foundFuel = false;

	int closestDistance = 999; //variable to track the distance to the closest unknown, absurdly high so that any valid unknown will be closer than it
	for (int i = 0; i < MAP_SIZE; i++) //loop through the x coordinates
	{
		for (int j = 0; j < MAP_SIZE; j++) //secondary loop through y coordinates (do all y values for x=0, then all y values for x=1, etc)
		{
			if (mapArray[i][j] == FUEL) //if the car found a visible fuel station
			{
				foundFuel = true; //update foundFuel so we know which priority to use later
				targetX = i; //update target x position
				targetY = j; //update target y position
			}

			//if it's not a fuel station, check if its an unknown tile UNLESS we have found a fuel station already
			else if (mapArray[i][j] == UNKNOWN && foundFuel==false)
			{
				//find the distance from the car's current position to the Unknown tile
				int tileX = i; //int to track x value of tile we're testing
				int tileY = j; //int to track y value of tile we're testing

				int distanceA = abs(carXPos - tileX); //first half of distance (done seperately to simplify code visually)
				int distanceB = abs(carYPos - tileY); //second half of distance (done seperately to simplify code visually)

				int distance = distanceA + distanceB; //combine the two distance components to have a final value

				if (distance < closestDistance) //if the distance to the UNKNOWN tile we just tested is shorter than the previously recorded shortestDistance
				{
					closestDistance = distance; //update closestDistance 
					targetX = i; //update x value of target tile
					targetY = j; //update y value of target tile
				}
				
			}

			
		}
	}

	//PART TWO: Execute the determined move

	//Check if we're lined up on the x axis with our target
	
	if (carXPos != targetX)
	{
		//check if we need to move right or left
		if (carXPos > targetX)
		{
			//we need to move to the left
			carXPos = carXPos - 1;
		}

		else if (carXPos<targetX)
		{
			//we need to move to the right
			carXPos = carXPos + 1;
		}
	}

	//if x is lined up, then y shouldn't be, or we'd already have a different target to line up with on the x axis

	else
	{
		//check if the car needs to move up or down

		if (carYPos > targetY)
		{
			//if the car is above the target, it needs to move down
			carYPos = carYPos - 1;
		}

		else if (carYPos < targetY)
		{
			//if the car is below the target, it needs to move up
			carYPos = carYPos + 1;
		}
	}

	numMoves++; //increment the move counter for the current car

	checkCar(); //call checkCar function to check if simulation needs to end for current car

	return true;

}

//function to check for various effects/conditions after the car moves
bool car::checkCar()
{
	/*

	This function should check for a few conditions in order. Firstly, check if we landed on a fuel station and need a refill, then if we have run out of gas and have reached
	game over for the current car.

	1.) Check the value of the current tile that the car is on
		a.) If value=FUEL (1), we get a refill for that car and need to update the value of currentFuelStation so that future updateMap calls will use the next station
			aa.) currentFuel=CAR_MAX_FUEL
			bb.) currentFuelStation++
		b.) Else, do nothing (obstacles not added yet)
	2.) Check if we've run out of fuel
		a.) If currentFuel<=0, set value of hasFinished to true so that the program finishes running the current car
	3.) Return true so that the function exits normally
	*/

	int identifier = mapArray[carXPos][carYPos]; //int variable for simplifying process of checking the enum value for the tile the car is on

	//check if we're on a fuel station tile
	if (identifier == FUEL) 
	{
		//update that tile value to no longer be a fuel station
		mapMasterArray[carXPos][carYPos] = EMPTY;

		//first, refill the car's fuel
		currentFuel = CAR_MAX_FUEL;

		//Update the value for currentFuelStation so that the next updateMap call uses the next fuelStation coordinates in the series
		fuelIndicator++;

	}

	//Check if we've run out of fuel for the current car
	if (currentFuel == 0)
	{
		//update hasFinished boolean variable to indicate that the current car has finished the simulation
		hasFinished = true;
	}

	//check if we've hit 100 moves

	if (numMoves >= 100)
	{
		hasFinished = true; //set hasFinished true so we can exit the while loop in runSimulation
	}

	//clear the map of any past car positions

	for (int g = 0; g < MAP_SIZE; g++) 
	{
		for (int f = -0; f < MAP_SIZE; f++)
		{
			if (mapArray[f][g] == CAR)
			{
				mapArray[f][g] = EMPTY;
			}
		}
	}

	mapArray[carXPos][carYPos] = CAR; //update the map to indicate where the car currently is

	return true; //return true so that the function exits normally
} //DONE

//function to update the car's map w/in its field of vision
bool car::updateMap()
{

	//	cout << "Started the updateMap function" << endl << endl;
		/*
		carXPos and carYPos indicate where the car currently is located on the map. The car's field of vision covers a single tile above, to the left of, and to the right of itself.

		DIRECTIONAL MATH NOTES

		TILE LEFT OF CAR: mapArray[carXPos-1][carYPos] IF carXPos!=0
		TILE ABOVE CAR: mapArray[carXPos][carYPos+1] IF carYPos!=11
		TILE RIGHT OF CAR: mapArray[carXPos+1][carYPos] IF carXPos!=11
		TILE BELOW CAR: mapArray[carXPos][carYPos-1] IF carYPos!=0

		*/

	//STEP ONE: Make sure we have the correct fuel station on the reference map

	//grab the coordinates of our current fuel station from the fuel document we made earlier

	int correctFuelX, correctFuelY; //int variables to hold the values for the correct X and Y of the current fuel station

	bool foundStation = false; //bool for controlling the loop if we find a visible fuel station

	fstream fin; //set up file input
	fin.open("coordinates.txt"); //open the file we made earlier when generation the random coordinates for the fuel station sequence
	
	//while we haven't found a fuel station
	while (foundStation == false)
	{
		while (!fin.eof()) //while we haven't reached the end of the document
		{
			fin >> testX >> testY >> id; //grab the x and y coordinate of the current line in the document, along with its identifier
			if (id == fuelIndicator) //if we have an identifier matching the indicator, we found the correct coordinates
			{
				correctFuelX = testX; //update correctFuelX
				correctFuelY = testY; //update correctFuelY
				foundStation = true; //set foundStation true so we can exit the loop
			}
		}
	}


	//set the position w/in the reference map at correctFuelX and correctFuelY to equal 2 (the enum for fuel stations)
	mapMasterArray[correctFuelX][correctFuelY] = 2;

	int testValue = mapArray[correctFuelX][correctFuelY]; //grab the value of the board at correct fuel x and y

	//check if we need to update a visible tile to make it a fuel station
	if (testValue != UNKNOWN) 
	{
		mapArray[correctFuelX][correctFuelY] = FUEL; //update the enum value for the visible tile
	}

	//print the coordinates of the current fuel station to the screen
	cout << "Current fuel station location: x= " << correctFuelY << " y= " << correctFuelX << endl << endl; 

	//STEP TWO: Update the car's map to include its current field of view by checking if any spaces w/in the appropriate range of the cars position are unknown tiles

	int newValue; //int to store the value to update the tiles to

	//update the space to the left of the car
	if (carXPos != MAP_LEFT_EDGE)
	{
		newValue = mapMasterArray[carXPos - CAR_VIEW_RANGE][carYPos];
		mapArray[carXPos - CAR_VIEW_RANGE][carYPos] = newValue;
	}
	
	//update the space above the car
	if (carYPos != MAP_TOP_EDGE)
	{
		newValue = mapMasterArray[carXPos][carYPos - CAR_VIEW_RANGE];
		mapArray[carXPos][carYPos - CAR_VIEW_RANGE] = newValue;
	}

	//update the space to the right of the car
	if (carXPos != MAP_RIGHT_EDGE)
	{
		newValue = mapMasterArray[carXPos + CAR_VIEW_RANGE][carYPos];
		mapArray[carXPos][carYPos + CAR_VIEW_RANGE] = newValue;
	}

	//update the space below the car
	if (carYPos != MAP_BOTTOM_EDGE)
	{
		newValue = mapMasterArray[carXPos][carYPos + CAR_VIEW_RANGE];
		mapArray[carXPos][carYPos + CAR_VIEW_RANGE] = newValue;
	}

	return true;
}

int car::runSimulation()  //main function to run each car through the simulation
{
	cout << "Current car= " << currentCar << endl << endl; //output the car we're on

	//clear past run's fuel station from the map so we don't have it and the actual first model on the board simultaneously

	for (int a = 0; a < MAP_SIZE; a++)
	{
		for (int b = 0; b < MAP_SIZE; b++)
		{
			if (mapArray[a][b] == FUEL)
			{
				mapArray[a][b] = EMPTY;
			}
		}
	}

	hasFinished = false; //reset hasFinished to false so we can loop continuously until the current car runs out of fuel
	currentFuelStation = 11; //reset fuel Station to 11 to reference the proper enum identifier w/in the fuelMap
	
	numMoves = 0; //reset move counter to zero so we can accurately measure the movement of the new car
	currentFuel = CAR_MAX_FUEL; //reset currentFuel so that the new car starts the simulation w/ a full tank of gas
	currentCar++; //update value of currentCar so we know which car we are on
	carXPos = 0; //reset carX to spawn point at 0,0 
	carYPos = 0; //reset carY to spawn point at 0,0
	fuelIndicator = 0; //reset the fuelIndicator to reset the fuel station sequence for the new car

	while (!hasFinished)
	{
		//call updateMap function (FUNCTION WRITTEN)
		updateMap();
	
		//call findMove function
		findMove();

		currentFuel--; //update fuel counter now that we've moved the car 1 space
		
		//update tiles around the car before the map is printed to the screen

		//check left of the car
		if (carXPos != 0) 
		{
			int newValue = mapMasterArray[carXPos - 1][carYPos];
			mapArray[carXPos - 1][carYPos] = newValue;
		}
		//check above the car
		if (mapArray[carXPos][carYPos + CAR_VIEW_RANGE] == UNKNOWN && carYPos != 11)
		{
			mapArray[carXPos][carYPos + CAR_VIEW_RANGE] = mapMasterArray[carXPos][carYPos + CAR_VIEW_RANGE]; //set the value of the mapArray at space equal to the same space's contents in the reference map
		}

		//check to the right of the car
		if (mapArray[carXPos + CAR_VIEW_RANGE][carYPos] == UNKNOWN && carXPos != 11)
		{
			mapArray[carXPos + CAR_VIEW_RANGE][carYPos] = mapMasterArray[carXPos + CAR_VIEW_RANGE][carYPos]; //set value of space in car's map equal to the value of the same space in the reference map
		}

		//check below the car
		if (mapArray[carXPos][carYPos - CAR_VIEW_RANGE] == UNKNOWN && carYPos != 0)
		{
			mapArray[carXPos][carYPos - CAR_VIEW_RANGE] = mapMasterArray[carXPos][carYPos - CAR_VIEW_RANGE]; //update the space in the car's map to equal the value of the same space in the reference map
		}


		//output the car's current map to the screen so the user can track the progress of the simulation

		for (int i = 0; i < MAP_SIZE; i++) //guarenteed loop through x values on map
		{
			for (int j = 0; j < MAP_SIZE; j++) //secondary loop to iterate through all y values for the current x value
			{
				int content = mapArray[i][j]; //grab the content value for the current space in the map

				cout << "[" << content << "]"; //output the content for the current tile to the map on the screen
			}
			cout << endl; //now that we've ended the current x row, add a new line to seperate the next row from it
		}
		
		//FIXME: This call to checkCar might be unnecessary
		
		//call checkCar function
		checkCar();

		//output current car's stats below the map each time
		cout << "Moves taken: " << numMoves << endl;
		cout << "Fuel remaining: " << currentFuel << endl << endl;
		system("PAUSE");

	}

	//update value for numCars(numCars++) now that the car we were running in the simulation has finished	
	numCars++;

	//return numCars
	return numMoves;
}