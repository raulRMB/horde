#include "SEnemy.h"
#include "components/CEnemy.h"
#include "components/CFollow.h"
#include "components/CTransform.h"

namespace tX
{

void SEnemy::Update(float deltaSeconds)
{
    for(const entt::entity& entity : GetView<CEnemy, CFollow>())
    {
        Process(entity, Game::GetRegistry(), deltaSeconds);
    }
}

void SEnemy::Process(entt::entity e, entt::registry& r, float deltaSeconds) {
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
