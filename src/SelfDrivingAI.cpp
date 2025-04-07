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
    car.update(GetFrameTime(), track.worldSegments);
    car.updateData(track.worldSegments, track.checkpointSegments);
}

void drawDebug() {
    rlImGuiBegin();
    ImGui::SetWindowSize(ImVec2(400, 400));
    ImGui::Begin("Debug Menu", NULL);
    std::string text;
    if (ImGui::BeginTabBar("scene##left_tabs_bar")) {
        if (ImGui::BeginTabItem("Car Info")) {
            text = std::format("Position {}, {} Angle {}", car.pos.x, car.pos.y, car.rot);
            ImGui::Text(text.c_str());
            text = std::format("Velocity {}, {}, Mag {}", car.vel.x, car.vel.y, length(car.vel));
            ImGui::Text(text.c_str());
            text = std::format("dist to checkpoint {}", car.data.distToCheckPoint);
            ImGui::Text(text.c_str());

            if(ImGui::Button("Reset")) {
                car.reset(
                    { track.worldSegments[0].x + 35.0f , track.worldSegments[0].y - 50},
                    track.worldSegments,
                    track.checkpointSegments
                );
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Track Info")) {
            if (ImGui::Button("Recreate")) {
                track = RaceTrack(SCR_WIDTH, SCR_HEIGHT);
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Game Info")) {
            ImGui::Checkbox("Pause", &isPlaying);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
    rlImGuiEnd();
}

void render(Camera2D mainCamera) {
    
}


int main(void)
{
    // Initialization

    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Self Driving Car");
    Camera2D mainCamera = { { SCR_WIDTH * 0.5f,SCR_HEIGHT * 0.5f }, { SCR_WIDTH * 0.5f,SCR_HEIGHT * 0.5f }, 0, 1};

    //We use this because in for scope we do not need to maximize the amount of frame we render at we can change this later
    SetTargetFPS(60); 

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
    config.StepSize() = 0.001;
    config.Discount() = 0.95;
    config.TargetNetworkSyncInterval() = 100;
    config.ExplorationSteps() = 1000;
    config.DoubleQLearning() = true;
    config.StepLimit() = 200; // Max steps per episode
 
    QLearning<CarTrackEnv, decltype(model), AdamUpdate, decltype(policy)> agent(config, model, policy, replay);
    CarTrackEnv* env = &agent.Environment();
    while (!WindowShouldClose()) {


        //car.updateInput();
        //if(isPlaying) simulate();
        
        auto episode = agent.Episode();

        
        BeginDrawing();
        BeginMode2D(mainCamera);

        ClearBackground(BLACK);


        env->track.draw();
        env->car.draw();
        EndMode2D();
        drawDebug();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
