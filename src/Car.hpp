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
	struct VisionData {
		vec2 position; // 2
		float speed; // 1
		vector<float> distToWall;
		float timeAlive;
		float distToCheckPoint;
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
	vector<vec2> hitBoxRays;
	vec2 checkPointRay;
	vec2 velocityRay;
	
	bool hasCollided = false;
	bool hasReachCheckpoint = false;
	bool checkpointTouched = false;
	bool collisionTouched = false;
	float turnSpeed = 50.0f;
	float speed = 4.75f;
	float dragCo = 0.98f;
	float rot = 90.0f;
	Color color = GREEN;
	Input input;
	VisionData data;

	Car() : Car(
		{ 0.0f,0.0f }, 
		vector<vec4>(0), 
		vector<vec4>(0)
	) 
	{};

	Car(vec2 startingPoint, vector<vec4> world, vector<vec4> checkpoints) : Car(GREEN, startingPoint, world, checkpoints) {};
	Car(Color c, vec2 startingPoint, vector<vec4> world, vector<vec4> checkpoints);
	void draw();
	void updateInput();
	void update(float dt, vector<vec4> worldSegments, vector<vec4> checkPoints);
	void updateData(vector<vec4> worldSegments, vector<vec4> checkPoints);
	void checkCollideWithWall(vector<vec4> worldSegments);
	void checkReachCheckpoint(vector<vec4> worldCheckpoints);
	void reset(vec2 point, vector<vec4> worldSegments, vector<vec4> checkPoints);

	vector<vec2> getRotatedHitBox();

};

