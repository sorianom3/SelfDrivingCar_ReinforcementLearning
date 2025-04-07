#include "CarTrackEnv.hpp"
#include "Util.hpp"

CarTrackEnv::CarTrackEnv() {
	
	track = RaceTrack(800.0f,800.0f);
	car = Car(
		{ track.worldSegments[0].x + 35.0f , track.worldSegments[0].y - 50 },
		track.worldSegments,
		track.checkpointSegments
	);

	car.isAIControl = true;
	crashed = false;
	reachCheckpoint = false;

	state = InitialSample();
	stepSize = 0.01;
}

void CarTrackEnv::SyncData() {
	state.SetData(car);
}



CarTrackEnv::State CarTrackEnv::InitialSample() {
	//reset simulation vars
	stepCount = 0;
	track = RaceTrack(800.0f, 800.0f);
	car = Car(
		{ track.worldSegments[0].x + 35.0f , track.worldSegments[0].y - 50 },
		track.worldSegments,
		track.checkpointSegments
	);
	state.ResetData();
	SyncData();

	return state;
}

void CarTrackEnv::SimulatePhysics(Action action) {
	car.input.drive = (action.action == Action::FOWARD) - (action.action == Action::BACKWARD);
	car.input.turn = (action.action == Action::LEFT) - (action.action == Action::RIGHT);

	//TODO CAHNGE HARDCODED DELTATIME
	car.update(stepSize, track.worldSegments);
	car.updateData(track.worldSegments, track.checkpointSegments);
	SyncData();
}
bool CarTrackEnv::CheckCheckpoint(vector<glm::vec4> checkpointSegments) {
	return false;
}

bool CarTrackEnv::IsTerminal(const State& state) {

	return crashed || stepCount >= maxSteps;
}
double CarTrackEnv::Reward(const State& state, const Action& action) {
	if (crashed)
		return -100.0;

	if (reachCheckpoint)
		return +100.0;

	double wallDistance = std::min({ state.data(3), state.data(4), state.data(5), state.data(6) });
	double distToGoal = state.data(7);
	double speed = state.data(2);

	return 1.0 * speed - 0.05 * distToGoal + 0.1 * wallDistance;
}

double CarTrackEnv::Sample(const State& state, const Action& action, State& nextState) {
	stepCount++;
	SimulatePhysics(action);

	crashed = car.hasCollided;
	reachCheckpoint = car.data.distToCheckPoint <= 0.0f;

	double reward = Reward(state, action);
	bool done = IsTerminal(state);
	if (done && maxSteps != 0 && stepCount >= maxSteps)
		return 1.0; // done reward
	return reward;
}
double CarTrackEnv::Sample(const State& state, const Action& action)
{
	State nextState;
	return Sample(state, action, nextState);
}