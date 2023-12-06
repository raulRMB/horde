#ifndef SMARTENTITY_H
#define SMARTENTITY_H

#include <entt/entt.hpp>
#include "Game.h"

namespace tZ
{

class SmartEntity
{
private:
    entt::entity Entity = entt::null;

public:
    virtual void Start() = 0;
    virtual void Kill();

    SmartEntity() : Entity(Game::GetRegistry().create()) {}
    explicit SmartEntity(entt::entity entity) {
        Entity = entity;
    }
    virtual ~SmartEntity() = default;

    [[nodiscard]] entt::entity GetEntity() const { return Entity; }

    template<typename T>
    void AddComponent(T component)
    {
        Game::GetRegistry().emplace<T>(Entity, component);
    }

    template<typename T>
    void RemoveComponent()
    {
        Game::GetRegistry().remove<T>(Entity);
    }

    template<typename T>
    T& GetComponent()
    {
        return Game::GetRegistry().get<T>(Entity);
    }

    template<typename T>
    bool HasComponent()
    {
        return GetComponent<T>() != entt::null;
    }

    explicit operator bool() const { return Entity != entt::null; }
    explicit operator entt::entity() const { return Entity; }
};

}


#endif //SMARTENTITY_H
