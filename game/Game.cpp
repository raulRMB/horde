#include "Game.h"
#include "scenes/MainScene.h"
#include "scenes/TestScene.h"
#include "smartentity/Player.h"
#include <thread>
#include <chrono>
#include "networking/base/NetworkDriver.h"
#include "util/raylibEx.h"
#include "components/Transform.h"
#include "imgui.h"
namespace raylib
{
#include "rlImGui.h"
}

#include "tools/SystemViewer.h"

namespace tZ
{

constexpr double tickRate = 60.0;
constexpr long long periodMicroseconds = static_cast<long long>(1e6 / tickRate);

Game::Game() : ActiveScene(nullptr), bRunning(false), BackgroundColor(0x00000000), fpsString(std::string("FPS: 0"))
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
    Instance().Spawn(networkId, t, owned);
}

void Game::Spawn(uint32_t networkId, CTransform& t, bool owned) {
    if(!IsServer()) {
        if(owned) {
            ownedPlayer = new Player();
            NetworkDriver::GetNetworkedEntities().Add(ownedPlayer->GetEntity(), networkId);
            ownedPlayer->SetTransform(t);
        } else {
            Player* player = new Player();
            NetworkDriver::GetNetworkedEntities().Add(player->GetEntity(), networkId);
            player->SetTransform(t);
        }
    }
}

Player* Game::GetPlayer() {
    return Instance().ownedPlayer;
}

void Game::Loop() {
    if(!Game::IsServer()) {
        HandleInput();
    }
    if(!Game::IsServer()) {
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
        //CalculateFPS();
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
    if(!Init())
    {
        return EXIT_FAILURE;
    }
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
        //raylib::SetTargetFPS(245);
        while (bRunning) {
            Loop();
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
    auto monitor = raylib::GetCurrentMonitor();
    int monitorWidth = raylib::GetMonitorWidth(monitor);
    int monitorHeight = raylib::GetMonitorHeight(monitor);
    raylib::SetWindowSize(monitorWidth , monitorHeight);
    raylib::ToggleFullscreen();
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
    if(!Game::IsServer() || serverDraw) {
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

void Game::ExecuteCommand(std::string cmd) {
    if(cmd == "toggle-fps") {
        showFPS = !showFPS;
    }
}

void Game::HandleCommandInput()
{
    int currKey = raylib::GetKeyPressed();
    if(currKey == raylib::KEY_ENTER) {
        ExecuteCommand(command);
        command = "";
        return;
    }
    bool shiftPressed = IsKeyDown(raylib::KEY_LEFT_SHIFT) || IsKeyDown(raylib::KEY_RIGHT_SHIFT);
    if (currKey != 0)
    {
        if(!shiftPressed) {
            if (currKey >= 'A' && currKey <= 'Z') {
                currKey += 'a' - 'A';
            }
        }
        if ((currKey >= 32) && (currKey <= 125)) {
            command += static_cast<char>(currKey);
        }
    }
    if(IsKeyPressed(raylib::KEY_BACKSPACE)) {
        if(!command.empty()) {
            command.pop_back();
        }
    }
}

void Game::HandleInput()
{
    if(IsKeyDown(raylib::KEY_F12))
    {
        bRunning = false;
    }

    if(IsKeyDown(raylib::KEY_LEFT_SHIFT) && IsKeyPressed(raylib::KEY_C)) {
        commandPromptOpen = !commandPromptOpen;
        return;
    }
    if(commandPromptOpen) {
        HandleCommandInput();
        return;
    }

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
    if(commandPromptOpen) {
        int screenWidth = raylib::GetScreenWidth();
        int screenHeight = raylib::GetScreenHeight();
        raylib::DrawRectangle(0, screenHeight - 20, screenWidth, 20, raylib::BLACK);
        std::string output = "> " + command;
        raylib::DrawText(output.c_str(), 3, screenHeight - 20, 13, raylib::WHITE);
    }

    raylib::rlImGuiBegin();

    SystemViewer::Instance().Draw();

    raylib::rlImGuiEnd();
}

void Game::Clean() const
{
    raylib::rlImGuiShutdown();
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

}