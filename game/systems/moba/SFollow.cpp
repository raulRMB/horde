#include "SFollow.h"

namespace raylib
{
#include "raylib.h"
#include "raymath.h"
}

#include "components/CFollow.h"

#include "glm/gtx/quaternion.hpp"

#include "util/raymathEx.h"
#include "components/CModel.h"
#include "components/CPhysics.h"
#include "components/CTransform.h"

namespace tX
{

void SFollow::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CFollow, CTransform, CPhysics2D>())
    {
        Process(entity, registry, deltaSeconds);
    }
}

void SFollow::Process(entt::entity e, entt::registry& r, float deltaSeconds) {
    CFollow& followComponent = r.get<CFollow>(e);
    EFollowState& followState = followComponent.FollowState;
    CPhysics2D& pc = r.get<CPhysics2D>(e);
    CTransform& t = r.get<CTransform>(e);

    if(followState != EFollowState::Following)
        return;

    if(pc.Velocity.x != 0 || pc.Velocity.y != 0) {
        v3 dir = v3{pc.Velocity.x, 0, pc.Velocity.y};
        dir = glm::normalize(dir);
        glm::quat rotation = glm::rotation(v3{0.0f, 0.0f, 1.0f}, dir);
        t.Rotation = rotation;
    }

    std::vector<v2>& stringPath = followComponent.StringPath;
    v2& targetPos = followComponent.TargetPos;
    unsigned int& followIndex = followComponent.Index;
    v3& followPos = r.get<CTransform>(e).Position;
    v2 followPos2d = {followPos.x, followPos.z};
    CPhysics2D& physics = r.get<CPhysics2D>(e);

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
            followComponent.FollowState = EFollowState::Idle;
            CTransform& t = r.get<CTransform>(e);
            t.Position = v3{targetPos.x, 0 , targetPos.y};
            physics.Velocity = {0.f, 0.f};
        }
    }
}

}
