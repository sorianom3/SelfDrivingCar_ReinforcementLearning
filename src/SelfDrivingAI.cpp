// SelfDrivingAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include <raylib.h>
#include "CarTrackEnv.hpp"
#include <string>
#include <format>



//GAME Variable
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 800;

RaceTrack track(SCR_WIDTH, SCR_HEIGHT);
Car car(
	{ track.worldSegments[0].x + 35.0f , track.worldSegments[0].y - 50 },
	track.worldSegments,
	track.checkpointSegments
);
bool isPlaying = true;


void simulate() {
	car.update(GetFrameTime(), track.worldSegments, track.checkpointSegments);
	car.updateData(track.worldSegments, track.checkpointSegments);
}

void drawDebug() {
	
}

void render(Camera2D mainCamera) {

}

void WriteRewardsToCSV(const std::vector<double>& rewards, const std::string& filename) {
	std::ofstream file(filename, std::ios::out);  // `std::ios::out` creates or overwrites the file

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << "\n";
		return;
	}

	// CSV header
	file << "Episode,TotalReward\n";

	// Write episode number and reward
	for (size_t i = 0; i < rewards.size(); ++i) {
		file << i << "," << rewards[i] << "\n";
	}

	file.close();
	std::cout << "Saved " << rewards.size() << " reward entries to " << filename << "\n";
}

int main(void)
{
	// Initialization

	InitWindow(SCR_WIDTH, SCR_HEIGHT, "Self Driving Car");
	Camera2D mainCamera = { { SCR_WIDTH * 0.5f,SCR_HEIGHT * 0.5f }, { SCR_WIDTH * 0.5f,SCR_HEIGHT * 0.5f }, 0, 1 };

	//We use this because in for scope we do not need to maximize the amount of frame we render at we can change this later
	//SetTargetFPS(60);

	rlImGuiSetup(true);
	ImGui::GetStyle().ScaleAllSizes(2);
	// Main game loop
	//test Q-learning


	FFN<MeanSquaredError, GaussianInitialization> network(MeanSquaredError(), GaussianInitialization(0, 0.001));
	network.Add<Linear>(128);
	network.Add<ReLU>();
	network.Add<Linear>(128);
	network.Add<ReLU>();
	network.Add<Linear>(4); // 4 actions

	SimpleDQN<> model(network);

	GreedyPolicy<CarTrackEnv> policy(1.0, 1000, 0.1, 0.99);
	
	RandomReplay<CarTrackEnv> replay(10, 1000);

	TrainingConfig config;
	config.StepSize() = 0.01;
	config.Discount() = 0.95;
	config.TargetNetworkSyncInterval() = 100;
	config.ExplorationSteps() = 1000;
	config.DoubleQLearning() = true;
	config.StepLimit() = 200; // Max steps per episode

	QLearning<CarTrackEnv, decltype(model), AdamUpdate, decltype(policy)> agent(config, model, policy, replay);
	CarTrackEnv* env = &agent.Environment();
	arma::running_stat<double> averageReturn;
	double totalReward = 0.0;
	int cycle = 100;
	vector<double> episodeRewards;
	while (!WindowShouldClose()) {

		CarTrackEnv::State nextState;
		agent.SelectAction();
		CarTrackEnv::Action action = agent.Action();
		double reward = env->Sample(env->curState, action, nextState);
		totalReward += reward;
		replay.Store(env->curState, action, reward, nextState, env->IsTerminal(nextState), config.Discount());
		agent.TrainAgent();
		env->curState = nextState;

		if (env->IsTerminal(env->curState)) {
			cout << "episode has Ended\n\n";
			std::cout << "Reward: " << totalReward << std::endl;
			episodeRewards.push_back(totalReward);
			totalReward = 0;
			env->InitialSample();
		}

		//if (env->car.hasCollided) {
		//	std::cout << "Epsiode Ended!\n";
		//	env->state = env->InitialSample();
		//}
		BeginDrawing();
		BeginMode2D(mainCamera);

		ClearBackground(BLACK);

		env->car.draw();
		env->track.draw();
		EndMode2D();
		#pragma region
		rlImGuiBegin();
		ImGui::SetWindowSize(ImVec2(400, 400));
		ImGui::Begin("Debug Menu", NULL);
		std::string text;
		if (ImGui::BeginTabBar("scene##left_tabs_bar")) {
			if (ImGui::BeginTabItem("Car Info")) {
				text = std::format("Position {}, {} Angle {}", env->car.pos.x, env->car.pos.y, env->car.rot);
				ImGui::Text(text.c_str());
				text = std::format("Velocity {}, {}, Mag {}", env->car.vel.x, env->car.vel.y, length(env->car.vel));
				ImGui::Text(text.c_str());
				text = std::format("dist to Wall TL: {} TR: {} BR: {} BL: {}",
					env->car.data.distToWall[0],
					env->car.data.distToWall[1],
					env->car.data.distToWall[2],
					env->car.data.distToWall[3]
				);
				ImGui::Text(text.c_str());
				text = std::format("dist to checkpoint {}", env->car.data.distToCheckPoint);
				ImGui::Text(text.c_str());

				if (ImGui::Button("Save Episodes' Rewards")) {
					WriteRewardsToCSV(episodeRewards, "episode_rewards.csv");
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
		rlImGuiEnd();
#pragma endregion Debug Menu
		EndDrawing();
		if (episodeRewards.size() > 400) {
			break;
		}
	}
	WriteRewardsToCSV(episodeRewards, "episode_rewards.csv");
	rlImGuiShutdown();
	CloseWindow();

	return 0;
}
