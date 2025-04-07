#include "Util.hpp"

vector<vec2> rotatePoints(vector<vec2> points, vec2 rPoint, float angle) {

	vector<vec2> output(points.size());
	float rads = radians(angle);
	for (int i = 0; i < points.size(); i++) {
		auto& p = points[i];
		float x = ((p.x - rPoint.x) * cosf(rads)) - ((p.y - rPoint.y) * sin(rads)) + rPoint.x;
		float y = ((p.y - rPoint.y) * cosf(rads)) + ((p.x - rPoint.x) * sinf(rads)) + rPoint.y;
		output[i].x = x;
		output[i].y = y;
	}
	return output;
}
float cross2D(vec2 a, vec2 b) {
	return a.x * b.y - a.y * b.x;
}

vec2 closetRayhit(vec2 startPoint, vec2 dir, vector<vec4> worldSegments) {
	float minT = INT_MAX;
	for (int i = 0; i < worldSegments.size(); i++) {
		auto a = vec2(worldSegments[i].x, worldSegments[i].y);
		auto b = vec2(worldSegments[i].z, worldSegments[i].w);

		auto m = b - a;

		auto r = startPoint;
		auto d = dir;

		auto t2 = (d.x * (a.y - r.y) + dir.y * (r.x - a.x)) / (m.x * dir.y - m.y * dir.x);
		auto t1 = (a.x + m.x * t2 - r.x) / dir.x;

		if (t1 >= 0 && t2 >= 0 && t2 <= 1) {
			minT = fminf(minT, t1);
		}
	}

	//float t2 = ((c.y - a1.y) * m1.x + (a1.x - c.x) * m1.y) / cross2D(n, m1);
	//float t1 = (c.x + n.x * t2 - a1.x) / m1.x;

	return startPoint + (dir * minT);
}
