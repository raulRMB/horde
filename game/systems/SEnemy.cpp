#include "SEnemy.h"

namespace raylib
{
#include "raylib.h"
}
#include "components/EnemyComponent.h"
#include "components/Follow.h"
#include "components/Transform.h"

namespace tZ
{

void SEnemy::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CEnemy, CFollow>())
    {
        CFollow& follow = GetComponent<CFollow>(entity);
        const v3& pos = GetComponent<CTransform>(Player).Position;
        raylib::TraceLog(raylib::LOG_WARNING, "Player %u", (u32)Player);
        follow.Goal = {pos.x, pos.z};
        follow.FollowState = EFollowState::Dirty;
        raylib::TraceLog(raylib::LOG_WARNING, "Enemy %f %f", follow.Goal.x, follow.Goal.y);
    }

    // for(const entt::entity entity : GetView<CEnemy, CTransform>())
    // {
    //     CTransform& transform = GetComponent<CTransform>(entity);
    //     raylib::TraceLog(raylib::LOG_WARNING, "Transform %f %f %f", transform.Position.x, transform.Position.y, transform.Position.z);
    // }
}

}
