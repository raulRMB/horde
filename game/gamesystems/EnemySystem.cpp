#include "EnemySystem.h"
#include "gamecomponents/EnemyComponent.h"
#include "components/Follow.h"

void EnemySystem::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CEnemy, CFollow>())
    {
        CFollow& follow = GetComponent<CFollow>(entity);
        const Vector3& pos = GetComponent<Transform>(Player).translation;
        follow.Goal = {pos.x, pos.z};
        follow.FollowState = EFollowState::Dirty;
    }
}