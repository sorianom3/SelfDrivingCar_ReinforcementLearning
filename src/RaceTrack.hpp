#pragma once
#include "Car.hpp"

class RaceTrack
{

	public:
		RaceTrack();
		void draw();
		bool checkIfCarCollided(Car car);
};
 
