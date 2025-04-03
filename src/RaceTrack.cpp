#include "RaceTrack.hpp"
#include <iostream>
#include "Util.hpp"

float randFloat(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
RaceTrack::RaceTrack(float width, float height) {
	srand(0);
	float radiusX = width * 0.25;
	float radiusY = height * 0.25;

	int checkPointCount = 9;
	worldSegments = vector<vec4>(checkPointCount * 2);
	auto worldVertices = vector<vec2>(checkPointCount * 2);

	for (int i = 0; i < checkPointCount; i++) {
		float theta = (i * PI * 2) / checkPointCount;
		float irregularity = 0.57f;
		float tempRadiusX = radiusX + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		float tempRadiusY = radiusY + (randFloat(-70.0f, 70.0f) * irregularity * 2 - irregularity);
		
		worldVertices[i] = {
			tempRadiusX * cosf(theta) + (width * 0.5f),
			tempRadiusY * sinf(theta) + (height * 0.5f)
		};
		worldVertices[checkPointCount + i] = {
			(tempRadiusX + 100.0f) * cosf(theta) + (width * 0.5f),
			(tempRadiusY + 100.0f) * sinf(theta) + (height * 0.5f)
		};
	}
	int mid = worldVertices.size() / 2;

	int j = 0;
	for (int i = 0; i < worldVertices.size(); i++) {
		
		int k = i + 1;
		if (i == mid - 1) {
			k = 0;
		}
		else if (i == worldVertices.size() - 1) {
			k = mid;
		}

		auto p1 = worldVertices[i];
		auto p2 = worldVertices[k];
		worldSegments[i] = {p1.x, p1.y, p2.x, p2.y};
	}
}

void RaceTrack::draw() {
	for (int i = 0; i < worldSegments.size();i++) {

		auto s = worldSegments[i];
		DrawLine(s.x, s.y, s.z, s.w, WHITE);
	}
}



bool RaceTrack::checkIfCarCollided(Car& car) {

	bool isHit = false;
	auto carHitBox = car.getRotatedHitBox();
	for (int i = 0; i < worldSegments.size() - 1; i++) {
		auto a1 = vec2(worldSegments[i].x, worldSegments[i].y);
		auto b1 = vec2(worldSegments[i].z, worldSegments[i].w);
		
		auto m = b1 - a1;
		
		for (int j = 0; j < carHitBox.size(); j++) {
			int k = (j == carHitBox.size() - 1) ? 0 : j + 1;
			auto c = carHitBox[j];
			auto d = carHitBox[k];
			auto n = d - c;
			
			//check inner interections
			float t2 = ((c.y - a1.y) * m.x + (a1.x - c.x) * m.y) / cross2D(n, m);
			float t1 = (c.x + n.x * t2 - a1.x) / m.x;

			isHit = isHit || (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1);

			if (isHit) break;
		}
		if (isHit) break;
	}
	car.hasCollided = isHit;
	return isHit;
}
