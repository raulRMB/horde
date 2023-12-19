#include "EnemySystem.h"
#include "components/Enemy.h"
#include "components/Follow.h"
#include "components/Transform.h"

namespace tZ
{

void EnemySystem::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CEnemy, CFollow>())
    {
        Process(entity, Game::GetRegistry(), deltaSeconds);
    }
}

void EnemySystem::Process(entt::entity e, entt::registry& r, float deltaSeconds) {
    CFollow& follow = r.get<CFollow>(e);
    CEnemy& enemy = r.get<CEnemy>(e);

    if(enemy.lastTarget != r.get<CTransform>(Player).Position) {
        enemy.lastTarget = r.get<CTransform>(Player).Position;
        const v3 &pos = r.get<CTransform>(Player).Position;
        follow.Goal = {pos.x, pos.z};
        follow.Index = 1;
        follow.FollowState = EFollowState::Dirty;
    }
}

}
