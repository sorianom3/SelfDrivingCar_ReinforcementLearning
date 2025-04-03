#include "RaceTrack.hpp"
#include <iostream>

float randFloat(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
RaceTrack::RaceTrack(float width, float height) {
	srand(0);
	float radiusX = width * 0.25;
	float radiusY = height * 0.25;

	int checkPointCount = 9;
	innerPoints = vector<vec2>(checkPointCount + 1);
	outerPoints = vector<vec2>(checkPointCount + 1);

	for (int i = 0; i < checkPointCount; i++) {
		float theta = (i * PI * 2) / checkPointCount;
		float irregularity = 0.57f;
		float tempRadiusX = radiusX + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		float tempRadiusY = radiusY + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		
		innerPoints[i] = {
			tempRadiusX * cosf(theta) + (width * 0.5f),
			tempRadiusY * sinf(theta) + (height * 0.5f)
		};
		outerPoints[i] = {
			(tempRadiusX + 100.0f) * cosf(theta) + (width * 0.5f),
			(tempRadiusY + 100.0f) * sinf(theta) + (height * 0.5f)
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

float cross2D(vec2 a, vec2 b) {
	return a.x * b.y - a.y * b.x;
}

float slope(vec2 a, vec2 b) {
	return (b.y - a.y) / (b.x - a.x);
}
bool RaceTrack::checkIfCarCollided(Car& car) {

	bool isHit = false;
	auto carHitBox = car.getRotatedHitBox();
	//for (int i = 0; i < innerPoints.size() - 1; i++) {
		int i = 0;
		auto a1 = innerPoints[i];
		auto b1 = innerPoints[i + 1];
		auto m1 = b1 - a1;
		auto m2 = outerPoints[i] - outerPoints[i+1];
		
		for (int j = 0; j < carHitBox.size(); j++) {
			int k = (j == carHitBox.size() - 1) ? 0 : j + 1;
			auto c = carHitBox[j];
			auto d = carHitBox[k];
			auto n = d - c;
			
			//check inner interection
			float t2 = ((c.y - a1.y) * m1.x + (a1.x - c.x) * m1.y) / (m1.y * n.x - n.y * m1.x);
			float t1 = (c.x + n.x * t2 - a1.x) / m1.x;

			car.view = vec2(t1, t2);
			isHit = isHit || (t1 >= 0 && t2 >= 0 && t2 <= 1);

			
		}
	//}
	car.hasCollided = isHit;
	return isHit;
}
