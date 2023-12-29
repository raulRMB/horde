#include "Scene.h"
#include "renderer/Renderer.h"
#include <vector>

namespace tX
{

void Scene::Start()
{

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
