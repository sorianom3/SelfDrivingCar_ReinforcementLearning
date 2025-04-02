// SelfDrivingAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include <raylib.h>
#include "Car.hpp"
#include "RaceTrack.hpp"
#include <string>
#include <format>

//GAME Variable
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 800;
RaceTrack track(SCR_WIDTH, SCR_HEIGHT);
Car car({ track.innerPoints[0].x + 35.0f , track.innerPoints[0].y });

void simulate() {
    car.update(GetFrameTime());
    track.checkIfCarCollided(car);
}

void drawDebug() {
    rlImGuiBegin();
    ImGui::SetWindowSize(ImVec2(400, 400));
    ImGui::Begin("Debug Menu", NULL);
    std::string text;
    if (ImGui::BeginTabBar("scene##left_tabs_bar")) {
        if (ImGui::BeginTabItem("Car Info")) {
            text = std::format("Position {}, {}", car.pos.x, car.pos.y);
            ImGui::Text(text.c_str());
            text = std::format("Velocity {}, {}", car.vel.x, car.vel.y);
            ImGui::Text(text.c_str());
            text = std::format("Velocity Mag {}", car.vel.x* car.vel.x + car.vel.y* car.vel.y);
            ImGui::Text(text.c_str());
            text = std::format("Angle {}", car.rotation);
            ImGui::Text(text.c_str());
            ImGui::DragFloat("Car Speed", &car.speed, 1, 1.0f, 500.0f);
            ImGui::DragFloat("Car Max Speed", &car.maxSpeed, 1, 10.0f, 500.0f);
            ImGui::DragFloat("Turn Speed", &car.turnSpeed, 1, 50.0f, 500.0f);
            ImGui::DragFloat("Drag", &car.dragCo, 0.001f, 0.001f, 1.0f);
            if(ImGui::Button("Reset")) {
                car.reset({ track.innerPoints[0].x + 35.0f , track.innerPoints[0].y});
                car.rotation = 90;
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Track Info")) {
            if (ImGui::Button("Recreate")) {
                track = RaceTrack(SCR_WIDTH, SCR_HEIGHT);
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
    rlImGuiEnd();
}

void render(Camera2D mainCamera) {
    BeginDrawing();
    BeginMode2D(mainCamera);

    ClearBackground(BLACK);
    car.draw();
    track.draw();
    EndMode2D();
    drawDebug();
    EndDrawing();
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
    while (!WindowShouldClose()) {
        car.updateInput();
        simulate();
        render(mainCamera);
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
