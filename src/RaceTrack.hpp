#pragma once
#include "Car.hpp"
#include <vector>
using namespace std;

class RaceTrack
{

	public:
		vector<vec4> worldSegments;
		RaceTrack(float width, float height);
		void draw();
		
		bool checkIfCarCollided(Car& car);

};
 
