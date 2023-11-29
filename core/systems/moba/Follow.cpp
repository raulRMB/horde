#include "Follow.h"

#include "raylib.h"
#include "raymath.h"
#include "components/Follow.h"
#include "util/raymathEx.h"
#include "components/Model.h"
#include "components/Physics.h"


void SFollow::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<CFollow, Transform, CPhysics2D, CModel>())
    {
        CModel& modelComponent = GetComponent<CModel>(entity);
        CPhysics2D& pc = GetComponent<CPhysics2D>(entity);
        Transform & t = GetComponent<Transform>(entity);

        Vector3 dir = {pc.Velocity.x, 0, pc.Velocity.y};
        dir = Vector3Normalize(dir);
        t.rotation.z = atan2(dir.x, -dir.z) + PI;
        modelComponent.model.transform = MatrixRotateXYZ(Vector3{PI/2, 0, t.rotation.z});
    }
    for(const entt::entity& entity : registry.view<CFollow, Transform, CPhysics2D>())
    {
        CFollow& followComponent = registry.get<CFollow>(entity);
        EFollowState& followState = followComponent.FollowState;

        if(followState != EFollowState::Following)
            continue;

        std::vector<Vector2>& stringPath = followComponent.StringPath;
        Vector2& targetPos = followComponent.TargetPos;
        unsigned int& followIndex = followComponent.Index;
        Vector3& followPos = registry.get<Transform>(entity).translation;
        Vector2 followPos2d = {followPos.x, followPos.z};
        CPhysics2D& physics = registry.get<CPhysics2D>(entity);

        if(constexpr float minDist = 0.3f; Vector2DistanceSqr(followPos2d, targetPos) > minDist * minDist)
        {
            Vector2 direction = targetPos - followPos2d;
            Vector2 nDirection = Vector2Normalize(direction);
            nDirection = Vector2Scale(nDirection, physics.Speed);
            physics.Velocity = Vector2Add(physics.Acceleration, nDirection);
            if(abs(Vector2Length(physics.Velocity)) > abs(Vector2Length(direction)))
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
