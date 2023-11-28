#include "Spawn.h"
#include "components/Spawner.h"
#include "components/Follow.h"
#include "gamecomponents/EnemyComponent.h"
#include "components/Shapes.h"
#include "components/Physics.h"

void SSpawn::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();

    for(const entt::entity& entity : registry.view<Transform, CSpawner>())
    {
        CSpawner& spawner = GetComponent<CSpawner>(entity);
        const Transform& transform = GetComponent<Transform>(entity);

        spawner.SpawnTimer += deltaSeconds;
        if(spawner.SpawnTimer >= spawner.SpawnRate)
        {
            spawner.SpawnTimer = 0.f;
            entt::entity e = CreateEntity();
            Transform entityTransform = transform;
            AddComponent(e, entityTransform);
            CFollow followComp{};
            AddComponent(e, followComp);
            CSphere sphereComponent{};
            sphereComponent.color = RED;
            sphereComponent.Radius = .5f;
            AddComponent(e, sphereComponent);
            CPhysics2D physics{};
            physics.Speed = 10.f;
            physics.MaxSpeed = 10.f;
            AddComponent(e, physics);
            CEnemy enemy{};
            AddComponent(e, enemy);
        }
    }
}