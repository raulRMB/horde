#include "EnemySystem.h"
#include "gamecomponents/EnemyComponent.h"
#include "components/FollowComponent.h"

void EnemySystem::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CEnemy, FollowComponent>())
    {
        FollowComponent& follow = GetComponent<FollowComponent>(entity);
        const Vector3& pos = GetComponent<Transform>(Player).translation;
        follow.Goal = {pos.x, pos.z};
        follow.FollowState = EFollowState::Dirty;
    }
}