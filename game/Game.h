#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <entt/entt.hpp>

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

    bool Run();

    bool Init();

    void Start() const;
    void HandleInput();
    void Update(float deltaTime) const;
    void Draw() const;
    void DrawUI() const;
    void CalculateFPS();

    void Clean() const;

    void SetActiveScene(Scene* scene);
    [[nodiscard]] inline Scene* GetActiveScene() const { return ActiveScene; }

    [[nodiscard]] Camera3D& GetActiveCamera() const { return Camera; }


    static entt::registry& GetRegistry();

    void Save();
    void Load();
};



#endif //GAME_H
