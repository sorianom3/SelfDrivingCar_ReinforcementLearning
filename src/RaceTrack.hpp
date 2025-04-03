#pragma once
#include <raylib.h>
#include <glm/glm.hpp>
#include <vector>
using namespace std;
using namespace glm;

class RaceTrack
{

	public:
		vector<vec4> worldSegments;
		RaceTrack(float width, float height);
		void draw();

};
 
