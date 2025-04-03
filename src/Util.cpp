#include "Util.hpp"

std::vector<vec2> rotatePoints(std::vector<vec2> points, vec2 rPoint, float angle) {

	std::vector<vec2> output(points.size());
	for (int i = 0; i < points.size(); i++) {
		auto& p = points[i];
		float x = ((p.x - rPoint.x) * cosf(radians(angle))) - ((p.y - rPoint.y) * sin(radians(angle))) + rPoint.x;
		float y = ((p.y - rPoint.y) * cosf(radians(angle))) + ((p.x - rPoint.x) * sinf(radians(angle))) + rPoint.y;
		output[i].x = x;
		output[i].y = y;
	}
	return output;
}

vec2 rayCast(vec2 startPoint, vec2 dir) {

	return vec2(0.0f);
}
