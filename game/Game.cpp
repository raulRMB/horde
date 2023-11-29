#include "Game.h"
#include "scenes/MainScene.h"
#include "scenes/TestScene.h"
#include "Player.h"
#include <thread>
#include <chrono>
#include "components/Follow.h"
#include "systems/moba/Navigation.h"
#include "networking/NetworkDriver.h"

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

void Game::SpawnPlayer(u_int32_t networkId) {
    Instance().Spawn(networkId);
}

void Game::Spawn(u_int32_t networkId) {
    if(!IsServer()) {
        ownedPlayer = new Player();
        NetworkDriver::GetNetworkedEntities().Add(ownedPlayer->GetEntity(), networkId);
    }
}

Player* Game::GetPlayer() {
    return Instance().ownedPlayer;
}

bool Game::Run(bool bServer)
{
    NetworkDriver::SetIsServer(bServer);
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
        NetworkDriver::ProcessQueue();

        if(!Game::IsServer()) {
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

bool Game::IsOfflineMode() {
    return NetworkDriver::IsOfflineMode();
}

void Game::Fullscreen() {
    auto monitor = GetCurrentMonitor();
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    SetWindowSize(monitorWidth , monitorHeight);
    ToggleFullscreen();
}

//Server* Game::GetServer() {
//    return Instance().server;
//}
//
//Client* Game::GetClient() {
//    return Instance().client;
//}

bool Game::Init()
{
    if(!Game::IsServer()) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(800, 600, "Horde");
        // Fullscreen();
    }
    bRunning = true;
    SetActiveScene(new MainScene());
    Start();

    NetworkDriver::Start();
    return true;
}

bool Game::IsServer() {
    return NetworkDriver::IsServer();
}

void Game::Start() const
{
    if(NetworkDriver::IsOfflineMode()) {
        SpawnPlayer(GetRandomValue(1, UINT32_MAX));
    }
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
    delete ownedPlayer;
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
