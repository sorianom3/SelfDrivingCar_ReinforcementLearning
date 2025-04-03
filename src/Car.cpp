#include "Car.hpp"
#include <iostream>
#include "Util.hpp"

Car::Car(vec2 startingPoint) {
	
	texture = LoadTexture("../Assets/car.png");

	pos = startingPoint;
	vel = { 0.0f,0.0f };
	acc = { 0.0f,0.0f };
	input = { 0.0f,0.0f };
	rotation = 45.0f;

	hitBox = std::vector<vec2>(4);
	

}

std::vector<vec2> Car::getRotatedHitBox() {

	return rotatePoints(hitBox, pos, rotation);
}


void Car::draw() {
	carBody.x = pos.x;
	carBody.y = pos.y;
	Rectangle src = {
		0,
		0,
		texture.width,
		texture.height
	};
	//DrawTexturePro(texture, src, carBody,{ carBody.width * 0.5f, carBody.height * 0.5f }, rotation,WHITE);

	DrawRectanglePro(carBody, { carBody.width * 0.5f, carBody.height * 0.5f }, rotation, (hasCollided)? RED : GREEN);
	auto hBox = getRotatedHitBox();

	//DEBUG
	//for (int i = 0; i < 4; i++) {
	//	int k = (i == 3) ? 0 : i + 1;
	//	DrawLine(hBox[i].x, hBox[i].y, hBox[k].x, hBox[k].y, (hasCollided) ? RED : GREEN);
	//}
}

void Car::reset(vec2 point) {
	pos = point;
	vel *= 0.0f;
	rotation = 0;
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
	
	//update hitbox
	hitBox[0] = { pos.x - carBody.width * 0.5f, pos.y - carBody.height * 0.5f };
	hitBox[1] = { pos.x + carBody.width * 0.5f, pos.y - carBody.height * 0.5f };
	hitBox[2] = { pos.x + carBody.width * 0.5f, pos.y + carBody.height * 0.5f };
	hitBox[3] = { pos.x - carBody.width * 0.5f, pos.y + carBody.height * 0.5f };

}

void Car::updateInput() {
	if (isAIControl) {

	}
	else {
		input.drive = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
		input.turn = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
}