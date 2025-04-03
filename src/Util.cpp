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
float cross2D(vec2 a, vec2 b) {
	return a.x * b.y - a.y * b.x;
}

vec2 rayCast(vec2 startPoint, vec2 dir) {


	//float t2 = ((c.y - a1.y) * m1.x + (a1.x - c.x) * m1.y) / cross2D(n, m1);
	//float t1 = (c.x + n.x * t2 - a1.x) / m1.x;

	return vec2(0.0f);
}
