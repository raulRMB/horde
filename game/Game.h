#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <entt/entt.hpp>
#include "components/CCamera.h"
#include "primitives/Color.h"
#include "components/CTransform.h"

namespace tX
{
class Game
{
    entt::entity OwnedPlayer;
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
    EngineMode mode;

    static Game& Instance();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static entt::entity GetPlayer();

    bool Run(EngineMode mode);

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
    static bool IsClient();
    static bool IsStandalone();

    void Loop();

    void Save() {}
    void Load() {}

};

}


#endif //GAME_H
