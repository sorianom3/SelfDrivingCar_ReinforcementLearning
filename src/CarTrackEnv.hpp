#pragma once
#include "mlpac.hpp"
#include <glm/glm.hpp>
#include <vector> 
#include "RaceTrack.hpp"
#include "Car.hpp"

using namespace std;
using namespace mlpack;
using namespace ens;
using namespace mlpack::rl;

class CarTrackEnv
{
public:

	class State {
	public:
		State() : data(dimension) {};
		arma::colvec data;
		static constexpr size_t dimension = 8;
		void SetData(Car& car) {
			data(0) = car.data.position.x;
			data(1) = car.data.position.y;
			data(2) = car.data.speed; // magnitude of velocity
			data(3) = car.data.distToWall[0]; // TL
			data(4) = car.data.distToWall[1]; // TR
			data(5) = car.data.distToWall[2]; // BR
			data(6) = car.data.distToWall[3]; // BL
			data(7) = car.data.distToCheckPoint;
		};
		void ResetData() {
			data = arma::colvec(stateSize, arma::fill::zeros);
		}
		const arma::colvec& Encode() const { return data; }
	};
	class Action {
	public:
		enum actions {
			FOWARD = 0,
			BACKWARD = 1,
			LEFT = 2,
			RIGHT = 3,
		};
		Action::actions action;
		static const size_t size = 4;
	};

	size_t stepCount = 0;
	size_t maxSteps = 200;
	static constexpr size_t actionSize = 4;
	static constexpr size_t stateSize = 8;

	bool crashed;
	bool reachCheckpoint;
	float stepSize = 0.0f;
	//game vars
	RaceTrack track;
	Car car;

	State state;

	CarTrackEnv();

	void SimulatePhysics(Action action); // new Car Update funciton
	double Sample(const State& state, const Action& action, State& nextState);
	double Sample(const State& state, const Action& action);
	bool CheckCheckpoint(vector<glm::vec4> checkpointSegments);

	State InitialSample(); // start State with Car vars
	void SyncData();
	std::tuple<State, double, bool>  Step(const Action& action);
	bool IsTerminal(const State& state);
	double Reward(const State& state, const Action& action);

};

