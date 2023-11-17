#include "FollowSystem.h"

#include <raylib.h>
#include <raymath.h>
#include "../Components.h"
#include "../../game/SmartEntity.h"

void FollowSystem::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : registry.view<FollowComponent, Transform>())
    {
        FollowComponent& follow = registry.get<FollowComponent>(entity);
        Vector2& targetPos = registry.get<FollowComponent>(entity).TargetPos;
        const Vector3 targetPos3d = {targetPos.x, 0.f, targetPos.y};
        Vector3& followPos = registry.get<Transform>(entity).translation;

        if(!follow.bFollow)
        {
            continue;
        }

        std::vector<Vector2>& StringPath = follow.StringPath;

        if(Vector3Distance(followPos, targetPos3d) > 0.01f)
        {
            followPos = Vector3Add(followPos, Vector3Scale(Vector3Subtract(targetPos3d, followPos), 2.0f * deltaSeconds));
        }
        else
        {
            follow.Index++;
            if(StringPath.size() > follow.Index)
            {
                targetPos = StringPath[follow.Index];
            }
        }
    }
}
