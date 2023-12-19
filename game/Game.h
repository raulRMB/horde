#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <entt/entt.hpp>
#include "components/Camera.h"
#include "primitives/Color.h"
#include "components/Transform.h"

namespace tZ
{
class Game
{
    entt::entity ownedPlayer;
    int FrameCount = 0;

    bool showFPS = true;

    std::chrono::high_resolution_clock::time_point LastFPSTime;
    std::chrono::high_resolution_clock::time_point CurrentTime;
    int FPS = 0;

    class Scene* ActiveScene;
    bool bRunning;

    FColor BackgroundColor;
    CCamera3D Camera{};

public:
    Game();

    static Game& Instance();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static entt::entity GetPlayer();

    bool Run(bool bServer);

    bool Init();

    void Start() const;
    void HandleInput();
    void Update(float deltaTime) const;
    void Draw() const;
    void DrawUI() const;
    void Fullscreen();

    void Clean() const;

    void SetActiveScene(Scene* scene);
    [[nodiscard]] inline Scene* GetActiveScene() const { return ActiveScene; }

    [[nodiscard]] CCamera3D& GetActiveCamera() { return Camera; }

    static entt::registry& GetRegistry();
    static entt::dispatcher& GetDispatcher();

    static void SpawnPlayer(uint32_t networkId, CTransform& t, bool owned);

    static bool IsServer();
    static bool IsOfflineMode();

    void Loop();

    void Save();
    void Load();

};

}


#endif //GAME_H
