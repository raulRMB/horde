#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"
#include "ui/Canvas.h"
#include "raylib.h"
#include "primitives/Triangles.h"
#include "navigation/Navigation.h"

class MainScene final : public Scene
{
    std::vector<Vector2> Points;

    std::vector<Edge2D> Portals = {};

    bool p1Active = true;
    class Player* GetActivePlayer();

    class Player* pPlayer;
    class Player* p2Player;



public:
    MainScene();
    ~MainScene() override;

private:
    Canvas* mainCanvas;
    void Start() override;
    void InitUI();
    void HandleInput() override;
    void Update(float deltaSeconds) override;
    void Draw() override;
    void DrawUI() override;
    void Clean() override;
    void Save() override;
    void Load() override;
};



#endif //MAINSCENE_H