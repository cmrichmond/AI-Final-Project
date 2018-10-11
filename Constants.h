#ifndef MAIN_H
#define MAIN_H



#include <iostream>
#include <random>
#include <string>
#include <cstdlib>


 //constant variables

const int MAP_SIZE = 15; //constant int to represent the x and y maximum values for the map
const int CAR_MAX_FUEL = 20; //constant int to represent the maximum fuel each car in the simulation can hold at once
const int NUM_FUEL_STATIONS = 10; //constant to represent how many fuel stations we should make
const int CAR_VIEW_RANGE = 1; // constant to represent how many tiles ahead, left, and right the car can see from itself
const int MAX_GENERATIONS = 4;

//map edge constants
const int MAP_TOP_EDGE = 0; //constant to represent the top edge of the board
const int MAP_BOTTOM_EDGE = MAP_SIZE; //constant to represent the bottom edge of the board
const int MAP_LEFT_EDGE = 0; ///constant to represent the leftmost edge of the board
const int MAP_RIGHT_EDGE = MAP_SIZE; //constant ro represent the rightmose edge of the board

//enums

enum TILETYPE { UNKNOWN=0, EMPTY=1, FUEL=2, HAZARD=3, CAR=4, OBSTACLE=5 }; //enum to be used in mapArray to identify tile types for the car
	
#endif