#ifndef SYSTEM_H
#define SYSTEM_H

#include "../../game/Game.h"

class System
{
public:
    virtual ~System() = default;

    template <typename T>
    static T& Get();
protected:
    static entt::entity CreateEntity() { return Game::GetRegistry().create(); }
    template <typename C>
    static C& GetComponent(const entt::entity& entity) { return Game::GetRegistry().get<C>(entity); }
    template <typename C>
    static void AddComponent(const entt::entity& entity, C& Component) { Game::GetRegistry().emplace<C>(entity, Component); }
};

template <typename T>
T& System::Get()
{
    static T instance;
    return instance;
}

class UpdateSystem : public System
{
public:
    virtual void Update(float deltaSeconds) = 0;
};

class DrawingSystem : public System
{
public:
    virtual void Draw() = 0;
};



#endif //SYSTEM_H
