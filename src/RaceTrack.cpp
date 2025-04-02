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

bool RaceTrack::checkIfCarCollided(Car car) {

	bool isHit = false;
	auto carHitBox = car.getRotatedHitBox();
	for (int i = 0; i < innerPoints.size() - 1; i++) {
		auto iedge = innerPoints[i] - innerPoints[i+1];
		auto oedge = outerPoints[i] - outerPoints[i+1];

		for (int j = 0; j < carHitBox.size(); j++) {
			int k = (j == carHitBox.size() - 1) ? 0 : j + 1;
			auto cedge = carHitBox[j] - carHitBox[k];

			float ixc = cross2D(iedge, cedge);
			float oxc = cross2D(oedge, cedge);

			auto imc = innerPoints[i + 1] - carHitBox[k];
			auto omc = outerPoints[i + 1] - carHitBox[k];

			float it = cross2D(imc, iedge) / ixc;
			float is = cross2D(imc, cedge) / ixc;
			float ot = cross2D(omc, oedge) / oxc;
			float os = cross2D(omc, cedge) / oxc;
			isHit = isHit || (
				( it >= 0 && it <= 1 && is >= 0 && is <= 1 ) ||
				( ot >= 0 && ot <= 1 && os >= 0 && os <= 1 ) 
				);
			car.hasCollided = isHit;
			std::cout << it << " " << is << "\n";
		}
	}
	
	return isHit;
}