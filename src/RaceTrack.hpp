#pragma once
#include "Car.hpp"
#include <vector>
using namespace std;

class RaceTrack
{

	public:
		vector<vec2> innerPoints;
		vector<vec2> outerPoints;
		RaceTrack(float width, float height);
		void draw();
		bool checkIfCarCollided(Car car);
};
 
