//
// Created by Raul Romero on 2023-11-15.
//

#include "Game.h"
#include <raylib.h>
#include "MainScene.h"

Game::Game() : CurrentScene(nullptr), bRunning(false), BackgroundColor(BLACK)
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

    SetScene(new MainScene());
    Start();

    return true;
}

void Game::Start() const
{
    CurrentScene->Start();
}

void Game::HandleInput()
{
    if(IsKeyDown(KEY_F12))
    {
        bRunning = false;
    }

    CurrentScene->HandleInput();
}

void Game::Update(float deltaTime) const
{
    CurrentScene->Update(deltaTime);
}

void Game::Draw() const
{
    BeginMode3D(Camera);
    CurrentScene->Draw();
    EndMode3D();
}

void Game::DrawUI() const
{
    CurrentScene->DrawUI();
}

void Game::Clean() const
{
    CurrentScene->Clean();
    delete CurrentScene;
}

void Game::SetScene(Scene* scene)
{
    CurrentScene = scene;
}
