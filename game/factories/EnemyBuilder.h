#ifndef B_ENEMYBUILDER_H
#define B_ENEMYBUILDER_H

#include "factories/Factory.h"

namespace tZ
{

class BEnemy : public Builder
{
public:
    BEnemy() = default;
    ~BEnemy() override = default;

    entt::entity Build(const CTransform& transform) override;
    entt::entity Build(const CTransform& transform, u32 netId) override;
};

}

#endif //ENEMYBUILDER_H
