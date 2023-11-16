#include "Scene.h"
#include "systems/AnimationSystem.h"
#include "systems/EnemySystem.h"
#include <vector>
#include "systems/ShapeDrawingSystem.h"

void Scene::Start()
{
    UpdateSystems.push_back(new EnemySystem());
    UpdateSystems.push_back(new AnimationSystem());

    DrawingSystems.push_back(new ShapeDrawingSystem());
}

void Scene::Update(float deltaSeconds)
{
    for (System* system : UpdateSystems)
    {
        system->Update(deltaSeconds);
    }
}

void Scene::Draw()
{
    for(DrawingSystem* system : DrawingSystems)
    {
        system->Draw();
    }
}
