#include "FollowSystem.h"

#include <raylib.h>
#include <raymath.h>
#include "../Components.h"
#include "../../game/SmartEntity.h"
#include "../util/raymathEx.h"

void FollowSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<FollowComponent, Transform, Physics2DComponent>())
    {
        FollowComponent& follow = registry.get<FollowComponent>(entity);
        Vector2& targetPos = registry.get<FollowComponent>(entity).TargetPos;
        Vector3& followPos = registry.get<Transform>(entity).translation;
        Vector2 followPos2d = {followPos.x, followPos.z};
        Physics2DComponent& physics = registry.get<Physics2DComponent>(entity);

        if(!follow.bFollow)
        {
            continue;
        }

        std::vector<Vector2>& StringPath = follow.StringPath;
        if(constexpr float minDist = 0.1f; Vector2DistanceSqr(followPos2d, targetPos) > minDist * minDist)
        {
            Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, followPos2d));
            direction = Vector2Scale(direction, physics.Speed);
            physics.Velocity = Vector2Add(physics.Acceleration, direction);
        }
        else
        {
            follow.Index++;
            if(StringPath.size() > follow.Index)
            {
                targetPos = StringPath[follow.Index];
            }
            else
            {
                follow.bFollow = false;
                physics.Velocity = {0.f, 0.f};
            }
        }
    }
}
