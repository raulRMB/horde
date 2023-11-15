//
// Created by Raul Romero on 2023-11-15.
//

#include "Game.h"
#include <raylib.h>
#include "MainScene.h"

Game::Game() : ActiveScene(nullptr), bRunning(false), BackgroundColor(BLACK)
{
    Camera.position = {0.0f, 10.0f, 10.0f};
    Camera.target = {0.0f, 0.0f, 0.0f};
    Camera.up = {0.0f, 1.0f, 0.0f};
    Camera.fovy = 45.0f;
    Camera.projection = CAMERA_PERSPECTIVE;
}

Game& Game::Instance()
{
    static Game instance;
    return instance;
}

bool Game::Run()
{
    if(!Init())
    {
        return false;
    }

    while(bRunning)
    {
        HandleInput();
        Update(GetFrameTime());
        BeginDrawing();
        ClearBackground(BackgroundColor);
        Draw();
        DrawUI();
        EndDrawing();
        bRunning = bRunning ? !WindowShouldClose() : false;
    }

    return true;
}

bool Game::Init()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Horde");
    bRunning = true;

    SetActiveScene(new MainScene());
    Start();

    return true;
}

void Game::Start() const
{
    ActiveScene->Start();
}

void Game::HandleInput()
{
    if(IsKeyDown(KEY_F12))
    {
        bRunning = false;
    }

    ActiveScene->HandleInput();
}

void Game::Update(float deltaTime) const
{
    ActiveScene->Update(deltaTime);
}

void Game::Draw() const
{
    BeginMode3D(Camera);
    ActiveScene->Draw();
    EndMode3D();
}

void Game::DrawUI() const
{
    ActiveScene->DrawUI();
}

void Game::Clean() const
{
    ActiveScene->Clean();
    delete ActiveScene;
}

void Game::SetActiveScene(Scene* scene)
{
    ActiveScene = scene;
}

entt::registry& Game::GetRegistry()
{
    return Instance().ActiveScene->GetRegistry();
}
