#pragma once
#include <raylib.h>
#include <glm/glm.hpp>

using namespace glm;
class Car
{
private:
	struct Input {
		float turn;
		float drive;
	};
public:
	
	bool isAIControl = false;

	Rectangle carBody = {
		0.0f,
		0.0f,
		25.0f,
		15.0f
	};
	vec2 pos;
	vec2 vel;
	vec2 acc;
	float turnSpeed = 165.0f;
	float speed = 10.0f;
	float maxSpeed = 15.0f;
	float dragCo = 0.96f;
	float rotation;
	Input input;


	Car(float scrWidth, float scrHeight);
	void draw();
	void updateInput();
	void update(float dt);

};

