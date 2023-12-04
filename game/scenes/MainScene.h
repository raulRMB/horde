#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.h"
#include "ui/Canvas.h"
#include "primitives/Triangles.h"

namespace tZ
{

class MainScene final : public Scene
{
    std::vector<Vector2> Points;

    std::vector<Edge2D> Portals = {};

    class Player* GetActivePlayer();

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

}

#endif //MAINSCENE_H
