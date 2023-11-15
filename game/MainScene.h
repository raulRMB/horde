#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "../Core/Scene.h"

class MainScene final : public Scene
{
public:
    MainScene();
    ~MainScene() override;

private:
    void Start() override;
    void InitUI();
    void HandleInput() override;
    void Update(float deltaSeconds) override;
    void Draw() override;
    void DrawUI() override;
    void Clean() override;
    void Save() override;
    void Load() override;

private:

};



#endif //MAINSCENE_H
