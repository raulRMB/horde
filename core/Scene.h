#ifndef SCENE_H
#define SCENE_H

#include <entt/entt.hpp>
#include "systems/System.h"

namespace tZ
{

class Scene
{
protected:
    entt::registry Registry;
    entt::dispatcher Dispatcher;
    std::vector<UpdateSystem*> UpdateSystems;
    std::vector<DrawingSystem*> DrawingSystems;

public:
    virtual ~Scene() = default;
    Scene() = default;

    virtual void Start();
    virtual void HandleInput() = 0;
    virtual void Update(float deltaSeconds);
    virtual void Draw();
    virtual void DrawUI() = 0;
    virtual void Clean();
    virtual void Save() = 0;
    virtual void Load() = 0;

    inline entt::registry& GetRegistry() { return Registry; }

    inline entt::dispatcher& GetDispatcher() { return Dispatcher; }

    entt::entity CreateEntity()
    {
        return Registry.create();
    }

    void RemoveEntity(entt::entity entity)
    {
        Registry.destroy(entity);
    }

    template<typename T>
    void AddComponent(entt::entity entity, T component)
    {
        Registry.emplace<T>(entity, component);
    }

    template<typename T>
    void RemoveComponent(entt::entity entity)
    {
        Registry.remove<T>(entity);
    }

    template<typename T>
    T& GetComponent(entt::entity entity)
    {
        return Registry.get<T>(entity);
    }
};

}

#endif //SCENE_H
