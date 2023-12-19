#include "Game.h"
#include "scenes/MainScene.h"
#include "scenes/TestScene.h"
#include <thread>
#include <chrono>
#include "networking/base/NetworkDriver.h"
#include "util/raylibEx.h"
#include "components/Transform.h"
#include "imgui.h"
#include "tools/SystemViewer.h"
#include "util/Builder.h"

namespace raylib
{
#include "rlImGui.h"
}

namespace tZ
{

constexpr double tickRate = 60.0;
constexpr long long periodMicroseconds = static_cast<long long>(1e6 / tickRate);

Game::Game() : ActiveScene(nullptr), bRunning(false), BackgroundColor(0x00000000)
{
    Camera.Position = {50.0f, 60.0f, 50.0f};
    Camera.Target = {0.0f, 0.0f, 0.0f};
    Camera.Up = {0.0f, 1.0f, 0.0f};
    Camera.Fovy = 55.0f;
    Camera.Projection = raylib::CAMERA_PERSPECTIVE;
}

Game& Game::Instance()
{
    static Game instance;
    return instance;
}

void Game::SpawnPlayer(uint32_t networkId, CTransform& t, bool owned) {
    if(!IsServer()) {
        if(owned) {
            Instance().ownedPlayer = Builder::Player(t);
            NetworkDriver::GetNetworkedEntities().Add(Instance().ownedPlayer, networkId);
        } else {
            entt::entity e = Builder::Player(t);
            NetworkDriver::GetNetworkedEntities().Add(e, networkId);
        }
    }
}

entt::entity Game::GetPlayer() {
    return Instance().ownedPlayer;
}

void Game::Loop() {
    if(!Game::IsServer()) {
        HandleInput();
        Update(raylib::GetFrameTime());
    }
    else {
        Update(1.0f / tickRate);
    }
    NetworkDriver::ProcessQueues();
    if(!Game::IsServer()) {
        raylib::BeginDrawing();
        ClearBackground(ToRaylibColor(BackgroundColor));
        Draw();
        DrawUI();
        if(showFPS) {
            raylib::DrawFPS(5, 5);
        }
        raylib::EndDrawing();
        bRunning = bRunning && !raylib::WindowShouldClose();
    }
}

bool Game::Run(bool bServer)
{
    NetworkDriver::SetIsServer(bServer);
    Init();
    if(IsServer()) {
        while (bRunning) {
            auto startTime = std::chrono::high_resolution_clock::now();
            Loop();
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
            auto sleepTime = periodMicroseconds - elapsedTime;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
            }
        }
    } else {
        while (bRunning) {
            Loop();
        }
    }
    return EXIT_SUCCESS;
}

bool Game::IsOfflineMode() {
    return NetworkDriver::IsOfflineMode();
}

void Game::Fullscreen() {
    auto monitor = raylib::GetCurrentMonitor();
    int monitorWidth = raylib::GetMonitorWidth(monitor);
    int monitorHeight = raylib::GetMonitorHeight(monitor);
    raylib::SetWindowSize(monitorWidth , monitorHeight);
    raylib::ToggleFullscreen();
}

bool Game::Init()
{
    if(!Game::IsServer()) {
        SetConfigFlags(raylib::FLAG_WINDOW_RESIZABLE);
        raylib::InitWindow(800, 600, "Horde");
        // Fullscreen();
        raylib::rlImGuiSetup(true);
    }
    bRunning = true;
    SetActiveScene(new MainScene());
    Start();
    NetworkDriver::Start(periodMicroseconds);
    return true;
}

bool Game::IsServer() {
    return NetworkDriver::IsServer();
}

void Game::Start() const
{
    if(NetworkDriver::IsOfflineMode()) {
        CTransform t = CTransform{};
        SpawnPlayer(1, t, true);
    }
    ActiveScene->Start();
}

void Game::HandleInput()
{
    ActiveScene->HandleInput();
}

void Game::Update(float deltaTime) const
{
    ActiveScene->Update(deltaTime);
}

void Game::Draw() const
{
    BeginMode3D(ToRaylibCamera(Camera));
    ActiveScene->Draw();
    raylib::EndMode3D();
}

void Game::DrawUI() const
{
    ActiveScene->DrawUI();
    raylib::rlImGuiBegin();
    SystemViewer::Instance().Draw();
    raylib::rlImGuiEnd();
}

void Game::Clean() const
{
    raylib::rlImGuiShutdown();
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

}