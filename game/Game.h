#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <entt/entt.hpp>

class Game
{
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

    void Clean() const;

    void SetActiveScene(Scene* scene);
    [[nodiscard]] inline Scene* GetActiveScene() const { return ActiveScene; }

    static entt::registry& GetRegistry();

    void Save();
    void Load();
};



#endif //GAME_H
