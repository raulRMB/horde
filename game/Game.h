#ifndef GAME_H
#define GAME_H

#include <raylib.h>

class Game
{
    class Scene* CurrentScene;
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

    void SetScene(Scene* scene);
    [[nodiscard]] inline Scene* GetCurrentScene() const { return CurrentScene; }

    void Save();
    void Load();
};



#endif //GAME_H
