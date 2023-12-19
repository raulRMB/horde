#include "SystemViewer.h"
#include "Game.h"
#include "imgui.h"
#include "systems/base/Physics.h"

namespace tZ
{

SystemViewer::SystemViewer()
{
}

SystemViewer::~SystemViewer()
{

}

SystemViewer &SystemViewer::Instance()
{
    static SystemViewer instance;
    return instance;
}


void SystemViewer::Update(float deltaSeconds)
{

}

void SystemViewer::Draw()
{
    ImGui::Begin("Systems");


    entt::registry& registry = Game::GetRegistry();

    for (entt::entity entity : System::Get<SPhysics>().GetAffectedEntities()) {
        const CTransform& transform = registry.get<CTransform>(entity);
        ImGui::Text("Transform: %f, %f, %f", transform.Position.x, transform.Position.y, transform.Position.z);
    }

    ImGui::End();
}

void SystemViewer::Init()
{

}

}