#include "Scene.h"
#include <vector>

#include "systems/base/SAnimation.h"
#include "systems/base/SCharacterAnimation.h"
#include "systems/moba/SFollow.h"
#include "systems/base/SPhysics.h"
#include "systems/base/SShapeDrawing.h"
#include "systems/base/SParticle.h"
#include "systems/base/SNetworking.h"
#include "systems/moba/SNavigation.h"
#include "systems/base/SModelDrawing.h"
#include "systems/moba/SAttribute.h"
#include "systems/moba/Spawn.h"
#include "systems/moba/SAttributeUI.h"
#include "systems/base/SParticleDrawing.h"
#include "systems/base/SLifetime.h"
#include "systems/SEnemy.h"

namespace tX
{

void Scene::Start()
{
    UpdateSystems.push_back(&System::Get<SNavigation>());
    UpdateSystems.push_back(&System::Get<SPhysics>());
    UpdateSystems.push_back(&System::Get<SSpawn>());
    UpdateSystems.push_back(&System::Get<SFollow>());
    UpdateSystems.push_back(&System::Get<SLifetime>());
    UpdateSystems.push_back(&System::Get<SEnemy>());

    if(!Game::IsStandalone()) {
        UpdateSystems.push_back(&System::Get<SNetworking>());
    }
    
    if(!Game::IsServer()) {
        DrawingSystems.push_back(&System::Get<SModelDrawing>());
        DrawingSystems.push_back(&System::Get<SShapeDrawing>());
        UpdateSystems.push_back(&System::Get<SAnimation>());
        UpdateSystems.push_back(&System::Get<SParticle>());
        DrawingSystems.push_back(&System::Get<SParticleDrawing>());
        DrawingSystems.push_back(&System::Get<SAttributeUI>());
    }

    if(!Game::IsClient()) {
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
