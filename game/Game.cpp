#include "Game.h"
#include "scenes/MainScene.h"
#include "scenes/TestScene.h"
#include "Player.h"
#include <thread>
#include <chrono>
#include "components/Follow.h"
#include "systems/moba/Navigation.h"

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
    Player* player = new Player();
    uint32_t netId = Instance().networkedEntities.Add(player->GetEntity());
    Game::GetServer()->ConnectResponse(peer, netId);
}

void Game::SpawnPlayer(u_int32_t networkId) {
    Instance().Spawn(networkId);
}

NetworkedEntities& Game::GetNetworkedEntities() {
    return Instance().networkedEntities;
}

void Game::Spawn(u_int32_t networkId) {
    if(!IsServer()) {
        ownedPlayer = new Player();
        networkedEntities.Add(ownedPlayer->GetEntity(), networkId);
    }
}

Player* Game::GetPlayer() {
    return Instance().ownedPlayer;
}

void Game::ProcessNetworkingQueue() {
    while(!GetNetworkingQueue().empty()) {
        enet_uint8 *data;
        data = GetNetworkingQueue().front();
        ENetMsg type = *(ENetMsg *) data;

        NetMessageTransform msg = *(NetMessageTransform *) data;
        InitialConnection x = *(InitialConnection *) data;

        if(IsServer()) {
            switch (type)
            {
                case ENetMsg::MoveTo:
                    if(System::Get<SNavigation>().IsValidPoint(msg.pos)) {
                        auto e = GetNetworkedEntities().Get(msg.NetworkId);
                        CFollow &followComponent = GetRegistry().get<CFollow>(e);
                        followComponent.FollowState = EFollowState::Dirty;
                        followComponent.Index = 1;
                        TraceLog(LOG_INFO, "x: %f y %f", msg.pos.x, msg.pos.y);
                        followComponent.Goal = msg.pos;
                    }
                    break;
                case ENetMsg::InitialConnection:
                    OnConnect(x.peer);
                    break;
                default:
                    TraceLog(LOG_INFO, "DEFAULT");
                    break;
            }
        } else {
            switch (type) {
                case ENetMsg::ConnectionResponse:
                    NetConnectionResponse msg = *(NetConnectionResponse *) data;
                    SpawnPlayer(msg.NetworkId);
                    TraceLog(LOG_INFO, "entt: %d", msg.NetworkId);
                    break;
            }
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
        ProcessNetworkingQueue();

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
    if(Game::IsOfflineMode()) {
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
