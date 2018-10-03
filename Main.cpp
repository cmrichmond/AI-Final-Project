#include<iostream>
#include<random>
#include<string>
#include"Constants.h"
#include"car.h"
#include<fstream>

using namespace std;

//Initialize Variables



//ADD LATER: OBSTACLES
//int obstacleLocationsArray[MAP_SIZE - 1][MAP_SIZE - 1]; //2d array to store x and y positions for each obstacle in the map

//Boolean variables for while loops

bool hasWon=false; //boolean variable to track if the simulation has reached its goal yet
bool hasFinished=false; //boolean variable to track if the current car in the simulation has crashed, run out of fuel, or finished

//tracking/index variables

int currentCar=0; //int variable to track the number of the current car running in the simulation
int numCars=0; //int variable to track the number of cars that have been run through the simulation
int currentFuelStation=0; //int variable to act as an index for the current fuel station
int numMoves=0; //int variable to keep track of how many successful moves the current car has made in the simulation


int main()
{
	int size = MAP_SIZE + 1;

	hasWon = false;
	car* newCar = new car;

	//Output program introduction to the screen

	cout << "Welcome to the simulation. This program will simulate an autonomous car trying to navigate a "<< size<< "x"<<size<<" map without running out of fuel" << endl << endl;

	cout << "The rules of this simulation are as follows:" << endl << endl;

	cout << "1. The car initially spawns on the map w/out any data on what its tiles contain." << endl << endl;;
	cout << "2. The car has a limited amount of fuel. Currently, the cars are set to start w/ a maximum fuel amount of " << CAR_MAX_FUEL << " ." << endl << endl;;
	cout << "3. Each time the car moves 1 tile, it consumes one unit of fuel" << endl << endl;;
	cout << "4. This program generates a random series of x and y coordinates for fuel stations to spawn on the map, but only one fuel station can exist at a time" << endl<<endl;
	cout << "5. Once the car reaches the first fuel station, it will be refueled to maximum fuel capacity, then the first fuel station will vanish and the next one will appear" << endl << endl;
	cout << "6. When the car is deciding what move to make, it will prioritize reaching a fuel station, if it can see the current one. If it cannot see a fuel station, the car will instead move towards the closest unknown space" << endl << endl;
	cout << "7. If a car runs out of fuel and did not end its turn on a fuel station, that car is retired from the simulation" << endl<<endl;
	cout << "8. The map data is not reset when a new car is started in the simulation, but the fuel station sequence is reset, i.e the first fuel station will appear in the same spot as it did in all previous runs" << endl<<endl;
	cout << "9. The simulation ends once a car has successfully moved 100 spaces without running out of fuel" << endl<<endl;
	cout << "!0. At a later point, the mechanics for obstacles and avoiding them will be added to this program as well" << endl << endl;
	
	system("PAUSE");


		//	cout << "Reached while loop for main's function" << endl << endl;


	newCar->Initialize();
	while (!hasWon)
	{
		//run a car through the simulation
		int resultScore=newCar->runSimulation();
		
		if (resultScore >= 100)
		{
			hasWon = true;
			cout << "Goal was successfully reached" << endl << endl;
			int duration = newCar->getNumCars();

			cout << "It took " << duration << " cars to reach the goal" << endl << endl;
		}

		else
		{
			cout << endl << endl;

			cout << "Car #" << currentCar << " failed. Starting a new car now" << endl << endl;
		}

		if (numCars >= MAX_GENERATIONS)
		{
			cout << "Ended due to generations cap" << endl << endl;
			hasWon = true;

		}
		
	}

	return 0;
	
}