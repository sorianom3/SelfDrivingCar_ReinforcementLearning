#include "RaceTrack.hpp"
#include <iostream>

float randFloat(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
RaceTrack::RaceTrack(float width, float height) {

	float radiusX = width * 0.25;
	float radiusY = height * 0.25;

	int checkPointCount = 8;
	innerPoints = vector<vec2>(checkPointCount + 1);
	outerPoints = vector<vec2>(checkPointCount + 1);

	for (int i = 0; i < checkPointCount; i++) {
		float theta = (i * PI * 2) / checkPointCount;
		float irregularity = 0.57f;
		float tempRadiusX = radiusX + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		float tempRadiusY = radiusY + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		
		innerPoints[i] = {
			tempRadiusX* cosf(theta) + (width * 0.5f),
			tempRadiusY* sinf(theta) + (height * 0.5f)
		};
		outerPoints[i] = {
			(tempRadiusX + 100.0f) *cosf(theta) + (width * 0.5f),
			(tempRadiusY + 100.0f)* sinf(theta) + (height * 0.5f)
		};
	}
	innerPoints[checkPointCount] = innerPoints[0];
	outerPoints[checkPointCount] = outerPoints[0];
}

void RaceTrack::draw() {

	for (int i = 0; i < innerPoints.size() - 1;i++) {
		auto ip1 = innerPoints[i];
		auto ip2 = innerPoints[i+1];
		auto op1 = outerPoints[i];
		auto op2 = outerPoints[i + 1];
		DrawLine(ip1.x, ip1.y, ip2.x, ip2.y, WHITE);
		DrawLine(op1.x, op1.y, op2.x, op2.y, WHITE);
	}
}

bool RaceTrack::checkIfCarCollided(Car car) {

	return false;
}