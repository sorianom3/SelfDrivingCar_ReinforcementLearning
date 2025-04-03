#pragma once
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

const vec2 UP = { 0.0f, 1.0f };

std::vector<vec2> rotatePoints(std::vector<vec2> points, vec2 rPoint, float angle);

vec2 rayCast(vec2 startPoint, vec2 dir);

float cross2D(vec2 a, vec2 b);
