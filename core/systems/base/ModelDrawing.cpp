#include "ModelDrawing.h"
#include "entt/entt.hpp"
#include "components/Model.h"
#include "components/Transform.h"
#include "raylibEx.h"

namespace tZ
{

void SModelDrawing::Draw()
{
    for(const entt::entity& entity : Game::GetRegistry().view<CModel>())
    {
        const CModel& modelComponent = Game::GetRegistry().get<CModel>(entity);
        const CTransform& t = Game::GetRegistry().get<CTransform>(entity);
        if(!modelComponent.hidden)
        {
            raylib::DrawModel(modelComponent.model, ToRaylibVector3(t.Position), modelComponent.scale, raylib::WHITE);
        }
    }
}

}
