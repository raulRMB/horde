#include "Follow.h"

#include "raylib.h"
#include "raymath.h"
#include "components/Follow.h"

#include <glm/gtx/norm.inl>

#include "util/raymathEx.h"
#include "components/Model.h"
#include "components/Physics.h"
#include "components/Transform.h"

namespace tZ
{

void SFollow::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CFollow, CTransform, CPhysics2D, CModel>())
    {
        CModel& modelComponent = GetComponent<CModel>(entity);
        CPhysics2D& pc = GetComponent<CPhysics2D>(entity);
        CTransform& t = GetComponent<CTransform>(entity);

        Vector3 dir = {pc.Velocity.x, 0, pc.Velocity.y};
        dir = Vector3Normalize(dir);
        t.Rotation.z = atan2(dir.x, -dir.z) + PI;
        modelComponent.model.transform = MatrixRotateXYZ(Vector3{PI/2, 0, t.Rotation.z});
    }
    for(const entt::entity& entity : registry.view<CFollow, CTransform, CPhysics2D>())
    {
        CFollow& followComponent = registry.get<CFollow>(entity);
        EFollowState& followState = followComponent.FollowState;

        if(followState != EFollowState::Following)
            continue;

        std::vector<v2>& stringPath = followComponent.StringPath;
        v2& targetPos = followComponent.TargetPos;
        unsigned int& followIndex = followComponent.Index;
        v3& followPos = registry.get<CTransform>(entity).Position;
        v2 followPos2d = {followPos.x, followPos.z};
        CPhysics2D& physics = registry.get<CPhysics2D>(entity);

        if(constexpr float minDist = 0.3f; glm::distance2(followPos2d, targetPos) > minDist * minDist)
        {
            v2 direction = targetPos - followPos2d;
            v2 nDirection = glm::normalize(direction);
            nDirection = nDirection *  physics.Speed;
            physics.Velocity = physics.Acceleration + nDirection;
            if(abs(physics.Velocity.length()) > abs(direction.length()))
            {
                physics.Velocity = direction / deltaSeconds;
            }
        }
        else
        {
            followIndex++;
            if(followIndex < stringPath.size())
            {
                targetPos = stringPath[followIndex];
            }
            else
            {
                followState = EFollowState::Idle;
                physics.Velocity = {0.f, 0.f};
            }
        }
    }
}

}
