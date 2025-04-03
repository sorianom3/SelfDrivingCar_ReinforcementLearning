#include "Car.hpp"
#include <iostream>
#include "Util.hpp"

Car::Car(vec2 startingPoint) {

	texture = LoadTexture("../Assets/car.png");

	pos = startingPoint;
	vel = { 0.0f,0.0f };
	acc = { 0.0f,0.0f };
	input = { 0.0f,0.0f };
	rotation = 90.0f;

	hitBox = vector<vec2>(4);


}

vector<vec2> Car::getRotatedHitBox() {

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

	DrawRectanglePro(carBody, { carBody.width * 0.5f, carBody.height * 0.5f }, rotation, (hasCollided) ? RED : GREEN);

	DrawLine(pos.x, pos.y, ray.x, ray.y, BLUE);

	//DEBUG
	//auto hBox = getRotatedHitBox();
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

void Car::update(float dt, vector<vec4> worldSegments) {

	rotation += (input.turn * length(vel) * turnSpeed * dt);
	rotation = fmodf(rotation, 360.0f);
	float pheta = radians(rotation);
	vec2 dir = { cosf(pheta),sinf(pheta) };

	float thrust = input.drive;
	float reverseReduction = 0.25f;
	if (thrust < 0) thrust *= reverseReduction;
	acc = dir * thrust * dt * speed;
	pos += vel;
	vel += acc;
	acc *= 0.0f;

	vel *= dragCo;
	float zeroThreshold = 0.015 * speed* reverseReduction;
	if (length(vel) <= zeroThreshold) {
		vel = vec2(0.0f);
	}
	ray = closetRayhit(pos, dir, worldSegments);

	//update hitbox
	hitBox[0] = { pos.x - carBody.width * 0.5f, pos.y - carBody.height * 0.5f };
	hitBox[1] = { pos.x + carBody.width * 0.5f, pos.y - carBody.height * 0.5f };
	hitBox[2] = { pos.x + carBody.width * 0.5f, pos.y + carBody.height * 0.5f };
	hitBox[3] = { pos.x - carBody.width * 0.5f, pos.y + carBody.height * 0.5f };

	checkCollideWithWall(worldSegments);
}

void Car::checkCollideWithWall(vector<vec4> worldSegments) {
	auto carHitBox = getRotatedHitBox();
	hasCollided = false;
	for (int i = 0; i < worldSegments.size() - 1; i++) {
		auto a = vec2(worldSegments[i].x, worldSegments[i].y);
		auto b = vec2(worldSegments[i].z, worldSegments[i].w);

		auto m = b - a;

		for (int j = 0; j < carHitBox.size(); j++) {
			int k = (j == carHitBox.size() - 1) ? 0 : j + 1;
			auto c = carHitBox[j];
			auto d = carHitBox[k];
			auto n = d - c;

			//check inner interections
			float t2 = ((c.y - a.y) * m.x + (a.x - c.x) * m.y) / cross2D(n, m);
			float t1 = (c.x + n.x * t2 - a.x) / m.x;

			hasCollided = hasCollided || (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1);

			if (hasCollided) break;
		}
		if (hasCollided) break;
	}
}
void Car::updateInput() {
	if (isAIControl) {

	}
	else {
		input.drive = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
		input.turn = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
	}
}