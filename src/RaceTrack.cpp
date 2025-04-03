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
	checkpointSegments = vector<vec4>(checkPointCount);
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
		checkpointSegments[i] = {
			worldVertices[i].x,
			worldVertices[i].y,
			worldVertices[checkPointCount + i].x,
			worldVertices[checkPointCount + i].y,
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

	for (int i = 0; i < checkpointSegments.size(); i++) {

		auto s = checkpointSegments[i];
		DrawLine(s.x, s.y, s.z, s.w, GREEN);
	}
}

