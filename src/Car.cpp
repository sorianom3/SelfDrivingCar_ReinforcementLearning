#include "Car.hpp"
vec2 UP = { 0.0f, 1.0f };
Car::Car(float scrWidth, float scrHeight) {
	pos = { scrWidth / 2.0f, scrHeight / 2.0f };
	vel = { 0.0f,0.0f };
	acc = { 0.0f,0.0f };
	input = { 0.0f,0.0f };
	rotation = 0.0f;
}

void Car::draw() {
	carBody.x = pos.x;
	carBody.y = pos.y;
	DrawRectanglePro(carBody, { carBody.width * 0.5f, carBody.height * 0.5f },rotation, RED);
}

void Car::update(float dt) {
	
	rotation += (input.turn * turnSpeed * dt);
	rotation = fmodf(rotation, 360.0f);
	float pheta = radians(rotation);
	vec2 dir = { cosf(pheta),sinf(pheta) };
	acc = dir * input.drive * dt * speed;
	pos += vel;
	if (vel.length() < maxSpeed) {
		vel += acc;
	}
	acc *= 0.0f;
	vel *= dragCo;
	
}

void Car::updateInput() {
	if (isAIControl) {

	}
	else {
		input.drive = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
		input.turn = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
}