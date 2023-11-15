#include "Scene.h"
#include "systems/AnimationSystem.h"
#include "systems/EnemySystem.h"
#include <vector>

void Scene::Start()
{
    Systems.push_back(new EnemySystem());
    Systems.push_back(new AnimationSystem());
}

void Scene::Update(float deltaSeconds)
{
    for (System* system : Systems)
    {
        system->Update(deltaSeconds);
    }
}
