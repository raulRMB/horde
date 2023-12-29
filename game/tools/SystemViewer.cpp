#include "SystemViewer.h"
#include "Game.h"
//#include "imgui.h"
#include "systems/base/SPhysics.h"

namespace tX
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
    static bool show = true;
//    ImGui::Begin("Systems", &show, ImGuiWindowFlags_MenuBar);
//
//    entt::registry& registry = Game::GetRegistry();
//
//    for (entt::entity entity : System::Get<SPhysics>().GetAffectedEntities()) {
//       auto transform = registry.try_get<CTransform>(entity);
//       if(transform != nullptr) {
//           ImGui::Text("Transform: %f, %f, %f", transform->Position.x, transform->Position.y, transform->Position.z);
//       }
//    }
//
//    ImGui::End();
}

void SystemViewer::Init()
{

}

}