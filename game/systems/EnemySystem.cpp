#include "SEnemy.h"
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
        follow.Goal = {pos.x, pos.z};
        follow.FollowState = EFollowState::Dirty;
    }
}

}
