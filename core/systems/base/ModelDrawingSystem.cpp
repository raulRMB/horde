#include "ModelDrawingSystem.h"
#include "entt/entt.hpp"
#include "components/Components.h"
#include "raymath.h"
#include "raylib.h"
#include "primitives/Triangles.h"

void ModelDrawingSystem::Draw()
{
    for(const entt::entity& entity : Game::GetRegistry().view<ModelComponent>())
    {
        const ModelComponent& modelComponent = Game::GetRegistry().get<ModelComponent>(entity);
        const Transform& t = Game::GetRegistry().get<Transform>(entity);
        if(!modelComponent.hidden)
        {
            DrawModel(modelComponent.model, t.translation, modelComponent.scale, WHITE);
        }
    }
}
