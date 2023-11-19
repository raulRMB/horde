#include "FollowSystem.h"

#include <raylib.h>
#include <raymath.h>
#include "components/Components.h"
#include "components/FollowComponent.h"
#include "../../game/SmartEntity.h"
#include "../util/raymathEx.h"


void FollowSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<FollowComponent, Transform, Physics2DComponent>())
    {
        FollowComponent& followComponent = registry.get<FollowComponent>(entity);
        EFollowState& followState = followComponent.FollowState;

        if(followState != EFollowState::Following)
            continue;

        std::vector<Vector2>& stringPath = followComponent.StringPath;
        Vector2& targetPos = followComponent.TargetPos;
        unsigned int& followIndex = followComponent.Index;
        Vector3& followPos = registry.get<Transform>(entity).translation;
        Vector2 followPos2d = {followPos.x, followPos.z};
        Physics2DComponent& physics = registry.get<Physics2DComponent>(entity);

        if(constexpr float minDist = 0.1f; Vector2DistanceSqr(followPos2d, targetPos) > minDist * minDist)
        {
            Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, followPos2d));
            direction = Vector2Scale(direction, physics.Speed);
            physics.Velocity = Vector2Add(physics.Acceleration, direction);
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
