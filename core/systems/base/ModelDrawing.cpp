#include "ModelDrawing.h"
#include "entt/entt.hpp"
#include "raymath.h"
#include "raylib.h"
#include "primitives/Triangles.h"
#include "components/Model.h"

void SModelDrawing::Draw()
{
    for(const entt::entity& entity : Game::GetRegistry().view<CModel>())
    {
        const CModel& modelComponent = Game::GetRegistry().get<CModel>(entity);
        const Transform& t = Game::GetRegistry().get<Transform>(entity);
        if(!modelComponent.hidden)
        {
            DrawModel(modelComponent.model, t.translation, modelComponent.scale, WHITE);
        }
    }
}
