#include "Scene.h"
#include <vector>

#include "systems/base/Animation.h"
#include "systems/base/CharacterAnimation.h"
#include "systems/moba/Follow.h"
#include "systems/base/Physics.h"
#include "systems/base/ShapeDrawing.h"
#include "systems/base/Particle.h"
#include "systems/base/Networking.h"
#include "systems/moba/Navigation.h"
#include "systems/base/ModelDrawing.h"
#include "systems/moba/Attribute.h"
#include "systems/moba/Spawn.h"
#include "systems/moba/AttributeUI.h"
#include "systems/base/ParticleDrawing.h"
#include "systems/base/Lifetime.h"
#include "systems/EnemySystem.h"

namespace tZ
{

void Scene::Start()
{
    UpdateSystems.push_back(&System::Get<SNavigation>());
    UpdateSystems.push_back(&System::Get<SPhysics>());
    UpdateSystems.push_back(&System::Get<SSpawn>());
    UpdateSystems.push_back(&System::Get<SNetworking>());
    UpdateSystems.push_back(&System::Get<SFollow>());
    UpdateSystems.push_back(&System::Get<SLifetime>());
    UpdateSystems.push_back(&System::Get<EnemySystem>());
    
    if(!Game::IsServer()) {
        DrawingSystems.push_back(&System::Get<SModelDrawing>());
        DrawingSystems.push_back(&System::Get<SShapeDrawing>());
        UpdateSystems.push_back(&System::Get<SAnimation>());
        UpdateSystems.push_back(&System::Get<SParticle>());
        DrawingSystems.push_back(&System::Get<SParticleDrawing>());
        DrawingSystems.push_back(&System::Get<SAttributeUI>());
    } else {
        UpdateSystems.push_back(&System::Get<SAttribute>());
        UpdateSystems.push_back(&System::Get<SCharacterAnimation>());
    }

}

void Scene::DrawUI()
{
    for(DrawingSystem* system : DrawingSystems)
    {
        system->Draw2D();
    }
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

}
