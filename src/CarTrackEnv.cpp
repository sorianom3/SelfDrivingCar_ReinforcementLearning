#include "CarTrackEnv.hpp"
#include "Util.hpp"

CarTrackEnv::CarTrackEnv() {
	
	track = RaceTrack(1000.0f,800.0f);
	car = Car(
		{ track.worldSegments[0].x + 50.0f , track.worldSegments[0].y - 75 },
		track.worldSegments,
		track.checkpointSegments
	);

	car.isAIControl = true;
	crashed = false;
	reachCheckpoint = false;
	stepSize = 0.01;
}

void CarTrackEnv::SyncData() {
	curState.SetData(car);
}

//std::tuple<CarTrackEnv::State, double, bool> CarTrackEnv::Step(const Action& action) {
//
//	stepCount++;
//	SimulatePhysics(action);
//
//	crashed = car.hasCollided;
//	reachCheckpoint = car.data.distToCheckPoint <= 0.0f;
//
//	double reward = Reward(state, action);
//	bool done = IsTerminal(state);
//	return std::make_tuple(state, reward, done);
//}


CarTrackEnv::State CarTrackEnv::InitialSample() {
	//reset simulation vars
	stepCount = 0;
	maxSteps = 200;
	track = RaceTrack(1000.0f, 800.0f);
	car = Car(
		{ track.worldSegments[0].x + 50.0f , track.worldSegments[0].y - 75 },
		track.worldSegments,
		track.checkpointSegments
	);
	curState.ResetData();
	SyncData();

	return curState;
}

void CarTrackEnv::SimulatePhysics(Action action) {
	car.input.drive =
		(
			action.action == Action::FOWARD ||
			action.action == Action::FR ||
			action.action == Action::FL
		) - (
			action.action == Action::BACKWARD ||
			action.action == Action::BR ||
			action.action == Action::BL
		);

	car.input.turn = 
		(
			action.action == Action::LEFT || 
			action.action == Action::BL	  ||
			action.action == Action::FL
		) - (
			action.action == Action::RIGHT ||
			action.action == Action::BR ||
			action.action == Action::FR
		);

	
	car.update(stepSize, track.worldSegments, track.checkpointSegments);
	car.updateData(track.worldSegments, track.checkpointSegments);
	//car.draw();
	SyncData();
}
bool CarTrackEnv::CheckCheckpoint(vector<glm::vec4> checkpointSegments) {
	return false;
}

bool CarTrackEnv::IsTerminal(const State& state) {

	bool hasHitWall = state.data(3) <= 0.0 || state.data(4) <= 0.0 || state.data(5) <= 0.0 || state.data(6) <= 0.0;

	return car.hasCollided;
}
double CarTrackEnv::Reward(const State& state, const Action& action) {
	double farthestWall = std::max({ state.data(3) , state.data(4) , state.data(5) , state.data(6) });
	double averageWallDist = (state.data(3) + state.data(4) + state.data(5) + state.data(6)) / 4.0;
	double distToGoal = state.data(7);
	double speed = state.data(2) / car.speed;

	// Normalize distances (assuming 0 - 800 for your screen dimensions)
	double checkpointBonus = car.hasReachCheckpoint ? 2.0 : 0.0;
	double collisionPenalty = car.hasCollided ? 1.0 : 0.0;


	// Shape reward
	double reward = 0.4 * speed
		+ 0.3 * (averageWallDist / farthestWall)
		+ 1.0 * checkpointBonus       // big reward for checkpoint
		- 1.0 * collisionPenalty;     // big penalty for crash


	return reward;
}

double CarTrackEnv::Sample(const State& state, const Action& action, State& nextState) {
	stepCount++;
	SimulatePhysics(action);

	nextState.SetData(car);

	double reward = Reward(state, action);
	//reward = fmin(fmax(reward, -1.0), 1.0);
	return reward;
}
double CarTrackEnv::Sample(const State& state, const Action& action)
{
	State nextState;
	return Sample(state, action, nextState);
}