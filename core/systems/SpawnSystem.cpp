#include "SpawnSystem.h"
#include "components/Components.h"
#include "components/SpawnerComponent.h"
#include "components/FollowComponent.h"
#include "gamecomponents/EnemyComponent.h"

void SpawnSystem::Update(float deltaSeconds)
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
            FollowComponent followComp{};
            AddComponent(e, followComp);
            SphereComponent sphereComponent{};
            sphereComponent.color = RED;
            sphereComponent.Radius = .5f;
            AddComponent(e, sphereComponent);
            Physics2DComponent physics{};
            physics.Speed = 10.f;
            physics.MaxSpeed = 10.f;
            AddComponent(e, physics);
            CEnemy enemy{};
            AddComponent(e, enemy);
        }
    }
}