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
		vector<vec4> checkpointSegments;
		
		RaceTrack(float width, float height);

		RaceTrack() : RaceTrack(100.0f, 100.0f) {};
		void draw();

};
 
