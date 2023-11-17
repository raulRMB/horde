#include "Scene.h"
#include "systems/AnimationSystem.h"
#include "systems/EnemySystem.h"
#include <vector>

#include "systems/FollowSystem.h"
#include "systems/ShapeDrawingSystem.h"

void Scene::Start()
{
    UpdateSystems.push_back(new EnemySystem());
    UpdateSystems.push_back(new AnimationSystem());
    UpdateSystems.push_back(new FollowSystem());

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

void Scene::Clean()
{
    for(const System* system : UpdateSystems)
    {
        delete system;
        system = nullptr;
    }
    UpdateSystems.clear();

    for(const DrawingSystem* system : DrawingSystems)
    {
        delete system;
        system = nullptr;
    }
    DrawingSystems.clear();
}