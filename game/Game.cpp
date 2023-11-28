//
// Created by Raul Romero on 2023-11-15.
//

#include "Game.h"
#include "scenes/MainScene.h"
#include "scenes/TestScene.h"
#include <thread>
#include <chrono>

Game::Game() : ActiveScene(nullptr), bRunning(false), BackgroundColor(BLACK)
{
    Camera.position = {50.0f, 50.0f, 50.0f};
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

bool Game::IsServer() {
    return Instance().isServer;
}

bool Game::IsOfflineMode() {
    return Instance().offlineMode;
}

void Game::OnConnect(ENetPeer* peer) {
    entt::entity connection = GetRegistry().create();
    Game::GetServer()->ConnectResponse(peer, connection);
}

void Game::SpawnPlayer(entt::entity networkId) {
    Instance().Spawn(networkId);
}

void Game::Spawn(entt::entity networkId) {
    ActiveScene->SpawnPlayer(networkId);
}

void Game::ProcessNetworkingQueue() {
    while(!GetNetworkingQueue().empty()) {
        auto data = GetNetworkingQueue().front();
        ENetMsg type = *(ENetMsg *) data;
        switch(type) {
            case ENetMsg::ConnectionResponse:
                NetConnectionResponse msg = *(NetConnectionResponse *) data;
                SpawnPlayer(msg.NetworkId);
                break;
        }
        GetNetworkingQueue().pop();
    }
}

bool Game::Run(bool bServer)
{
    isServer = bServer;
    if(!Init())
    {
        return EXIT_FAILURE;
    }

    while(bRunning)
    {
        
        if(!Game::IsServer()) {
            HandleInput();
        }

        Update(GetFrameTime());

        if(!Game::IsServer()) {
            ProcessNetworkingQueue();
            BeginDrawing();
            ClearBackground(BackgroundColor);
            Draw();
            DrawUI();
            CalculateFPS();
            EndDrawing();
            bRunning = bRunning && !WindowShouldClose();
        }
    }

    return EXIT_SUCCESS;
}

void Game::CalculateFPS()
{
    FrameCount++;
    CurrentTime = std::chrono::high_resolution_clock::now();
    if (const long long dt = std::chrono::duration_cast<std::chrono::seconds>(CurrentTime - LastFPSTime).count(); dt >= 1)
    {
        FPS = FrameCount / dt;
        fpsString = "FPS: " + std::to_string(FPS);
        FrameCount = 0;
        LastFPSTime = CurrentTime;
    }
}

void Game::Fullscreen() {
    auto monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    SetWindowSize(monitorWidth , monitorHeight);
    ToggleFullscreen();
}

Server* Game::GetServer() {
    return Instance().server;
}

Client* Game::GetClient() {
    return Instance().client;
}

void Game::InitNetworking() {
    if(Game::IsServer()) {
        server = new Server();
        std::thread networkThread([]()
      {
          double netFreq = 120.0;
          auto periodMicroseconds = static_cast<long long>(1e6 / netFreq);
          while(1)
          {
              auto startTime = std::chrono::high_resolution_clock::now();
              Game::GetServer()->Loop();
              auto endTime = std::chrono::high_resolution_clock::now();
              auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
              auto sleepTime = periodMicroseconds - elapsedTime;
              if (sleepTime > 0) {
                  std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
              }
          }
      });
        networkThread.detach();
    } else {
        client = new Client();
        std::thread networkThread([]()
        {
          double netFreq = 120.0;
          auto periodMicroseconds = static_cast<long long>(1e6 / netFreq);
          while(1)
          {
              auto startTime = std::chrono::high_resolution_clock::now();
              Game::GetClient()->Loop();
              auto endTime = std::chrono::high_resolution_clock::now();
              auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
              auto sleepTime = periodMicroseconds - elapsedTime;
              if (sleepTime > 0) {
                  std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
              }
          }
      });
        networkThread.detach();
    }
}

bool Game::Init()
{
    if(!Game::IsServer()) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(800, 600, "Horde");
        // Fullscreen();
    }
    while(!Game::IsServer() && !IsWindowReady()) {
        sleep(1);
    }
    bRunning = true;
    SetActiveScene(new MainScene());
    Start();

    InitNetworking();
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
    DrawText(fpsString.c_str(), 5, 5, 10, {237, 203, 102, 255});
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

entt::dispatcher& Game::GetDispatcher()
{
    return Instance().ActiveScene->GetDispatcher();
}
