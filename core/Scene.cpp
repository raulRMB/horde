#include "Scene.h"
#include <vector>

#include "systems/base/AnimationSystem.h"
#include "systems/moba/FollowSystem.h"
#include "systems/base/PhysicsSystem.h"
#include "systems/base/ShapeDrawingSystem.h"
#include "systems/base/ParticleSystem.h"
#include "systems/moba/NavigationSystem.h"
#include "systems/base/ModelDrawingSystem.h"
#include "systems/moba/AttributeSystem.h"
#include "systems/moba/SpawnSystem.h"

void Scene::Start()
{
    UpdateSystems.push_back(&System::Get<AnimationSystem>());
    UpdateSystems.push_back(&System::Get<NavigationSystem>());
    UpdateSystems.push_back(&System::Get<FollowSystem>());
    UpdateSystems.push_back(&System::Get<PhysicsSystem>());
    UpdateSystems.push_back(&System::Get<ParticleSystem>());
    UpdateSystems.push_back(&System::Get<AttributeSystem>());
    UpdateSystems.push_back(&System::Get<SpawnSystem>());

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