#include "Factory.h"

#include "EnemyBuilder.h"
#include "PlayerBuilder.h"

namespace tZ
{
entt::entity Builder::FinishBuilding()
{
    const entt::entity entity = CurrentEntity;
    CurrentEntity = entt::null;
    return entity;
}


Factory::Factory()
{
    AddBuilder(EBuilderType::Player, new BPlayer());
    AddBuilder(EBuilderType::Enemy, new BEnemy());
}

entt::entity Factory::Build(EBuilderType type, const CTransform& transform)
{
    return Get().Builders[type]->Build(transform);
}

entt::entity Factory::Build(EBuilderType type, const CTransform& transform, u32 netId)
{
    return Get().Builders[type]->Build(transform, netId);
}

void Factory::AddBuilder(const EBuilderType type, Builder* builder)
{
    Builders.emplace(type, builder);
}

}
