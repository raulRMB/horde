#include "ModelDrawing.h"
#include "entt/entt.hpp"
#include "components/Model.h"
#include "components/Transform.h"
#include "util/raylibEx.h"
#include "glm/ext/quaternion_trigonometric.hpp"

namespace tZ
{

void SModelDrawing::Draw()
{
    for(const entt::entity& entity : Game::GetRegistry().view<CModel>())
    {
        const CModel& modelComponent = Game::GetRegistry().get<CModel>(entity);
        const CTransform& t = Game::GetRegistry().get<CTransform>(entity);
        if(!modelComponent.bHidden)
        {
            glm::quat rotate = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
            auto fq = t.Rotation * rotate;
            float angle = glm::degrees(glm::angle(fq));
            glm::vec3 axis = glm::axis(fq);
            raylib::DrawModelEx(modelComponent.Model, ToRaylibVector3(t.Position), ToRaylibVector3(axis), angle, ToRaylibVector3(t.Scale), raylib::WHITE);
        }
    }
}

}
