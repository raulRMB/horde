#ifndef F_FACTORY_H
#define F_FACTORY_H

#include <entt/entt.hpp>
#include "Game.h"

namespace tZ
{

enum class EBuilderType : u32
{
        None = 0,
        Enemy,
        Player,
        Projectile
};


class Builder
{
        entt::entity CurrentEntity;
public:
        Builder() = default;
        virtual ~Builder() = default;

        void StartBuilding() { CurrentEntity =  Game::GetRegistry().create(); }

        [[nodiscard]] entt::entity GetCurrentEntity() const { return CurrentEntity; }
        entt::entity FinishBuilding();

        template<typename T>
        void AddComponent(T component)
        {
                Game::GetRegistry().emplace<T>(CurrentEntity, component);
        }

        template<typename T>
        T& GetComponent()
        {
                return Game::GetRegistry().get<T>(CurrentEntity);
        }

        virtual entt::entity Build(const CTransform& transform) = 0;
        virtual entt::entity Build(const CTransform& transform, u32 netId) = 0;
};

class Factory
{
std::map<EBuilderType, Builder*> Builders;

public:
        Factory();

        static Factory& Get()
        {
                static Factory instance;
                return instance;
        }

        static entt::entity Build(EBuilderType type, const CTransform& transform);
        static entt::entity Build(EBuilderType type, const CTransform& transform, u32 netId);
        void AddBuilder(EBuilderType type, Builder* builder);
};

}

#endif //F_FACTORY_H
