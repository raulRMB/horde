#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "../core/Scene.h"
#include "../core/ui/Canvas.h"
#include <raylib.h>
#include "../core/primitives/Triangles.h"
#include "../core/navigation/Navigation.h"

class MainScene final : public Scene
{
    std::vector<Vector2> Points;

    std::vector<Edge2D> Portals = {};
    class Player* pPlayer;
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
