#ifndef HORDE_TESTSCENE_H
#define HORDE_TESTSCENE_H

#include "Scene.h"

namespace tX
{

class TestScene : public Scene
{
public:
    void Start() override;
    void HandleInput() override;
    void Update(float deltaSeconds) override;
    void DrawUI() override;
    void Draw() override;
    void Clean() override;
    void Save() override {}
    void Load() override {}
};

}

#endif //HORDE_TESTSCENE_H
