#pragma once
#include <raylib.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;


class Car
{
private:
	struct Input {
		float turn;
		float drive;
	};

public:
	bool isAIControl = false;
	Texture2D texture;

	vector<vec2> hitBox;

	Rectangle carBody = {
		0.0f,
		0.0f,
		25.0f,
		15.0f
	};
	vec2 pos;
	vec2 vel;
	vec2 acc;
	vec2 ray;
	bool hasCollided = false;
	float turnSpeed = 50.0f;
	float speed = 4.75f;
	float dragCo = 0.98f;
	float rotation;
	Input input;


	Car(vec2 startingPoint);
	void draw();
	void updateInput();
	void update(float dt, vector<vec4> worldSegments);
	void checkCollideWithWall(vector<vec4> worldSegments);
	void reset(vec2 point);
	vector<vec2> getRotatedHitBox();

};

