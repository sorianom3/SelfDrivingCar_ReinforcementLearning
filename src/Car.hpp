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
	bool hasCollided = false;
	float turnSpeed = 165.0f;
	float speed = 10.0f;
	float maxSpeed = 15.0f;
	float dragCo = 0.96f;
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

