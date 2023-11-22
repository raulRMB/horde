#include "Scene.h"
#include <vector>

#include "systems/AnimationSystem.h"
#include "systems/EnemySystem.h"
#include "systems/FollowSystem.h"
#include "systems/PhysicsSystem.h"
#include "systems/ShapeDrawingSystem.h"
#include "systems/ParticleSystem.h"
#include "systems/NavigationSystem.h"
#include "systems/ModelDrawingSystem.h"
#include "systems/AttributeSystem.h"

void Scene::Start()
{
    UpdateSystems.push_back(&System::Get<AnimationSystem>());
    UpdateSystems.push_back(&System::Get<EnemySystem>());
    UpdateSystems.push_back(&System::Get<FollowSystem>());
    UpdateSystems.push_back(&System::Get<PhysicsSystem>());
    UpdateSystems.push_back(&System::Get<NavigationSystem>());
    UpdateSystems.push_back(&System::Get<ParticleSystem>());
    UpdateSystems.push_back(&System::Get<AttributeSystem>());

    DrawingSystems.push_back(&System::Get<ShapeDrawingSystem>());
    DrawingSystems.push_back(&System::Get<ModelDrawingSystem>());

}

void Scene::Update(float deltaSeconds)
{
    for (UpdateSystem* system : UpdateSystems)
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