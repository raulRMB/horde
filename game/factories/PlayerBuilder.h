#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include "Factory.h"

namespace tZ
{

class BPlayer : public Builder
{
public:
    BPlayer() = default;
    ~BPlayer() override = default;

    entt::entity Build(const CTransform& transform) override;
    entt::entity Build(const CTransform& transform, u32 netId) override;
};

}

#endif //PLAYERFACTORY_H
