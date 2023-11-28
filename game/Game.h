#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <chrono>
#include <entt/entt.hpp>
#include <queue>
#include <mutex>
#include "networking/Server.h"
#include "networking/Client.h"
#include "networking/NetMessage.h"

class Game
{
    int FrameCount = 0;
    std::chrono::high_resolution_clock::time_point LastFPSTime;
    std::chrono::high_resolution_clock::time_point CurrentTime;
    int FPS = 0;
    std::string fpsString;

    class Scene* ActiveScene;
    bool bRunning;

    Color BackgroundColor;

    Camera3D Camera{};

public:
    Game();

    static Game& Instance();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    std::map<entt::entity, ENetPeer> connections;

    bool isServer;
    bool offlineMode = false;

    static bool IsServer();
    static bool IsOfflineMode();
    Client* client;
    Server* server;

    bool Run(bool bServer);

    bool Init();

    void Start() const;
    void HandleInput();
    void Update(float deltaTime) const;
    void Draw() const;
    void DrawUI() const;
    void CalculateFPS();
    void Fullscreen();
    void InitNetworking();

    std::queue<enet_uint8*> networkingQueue;
    static std::queue<enet_uint8*>& GetNetworkingQueue() { return Instance().networkingQueue; };

    void Clean() const;

    void SetActiveScene(Scene* scene);
    [[nodiscard]] inline Scene* GetActiveScene() const { return ActiveScene; }

    [[nodiscard]] Camera3D& GetActiveCamera() { return Camera; }


    static entt::registry& GetRegistry();
    static entt::dispatcher& GetDispatcher();

    static Server* GetServer();
    static Client* GetClient();



    static void OnConnect(ENetPeer* peer);
    static void SpawnPlayer(entt::entity networkId);

    void Save();
    void Load();

    void Spawn(entt::entity networkId);

    static void ProcessNetworkingQueue();
};



#endif //GAME_H
