#include "Spawn.h"
#include "components/Spawner.h"
#include "components/Follow.h"
#include "components/EnemyComponent.h"
#include "components/Network.h"
#include "components/Shapes.h"
#include "components/Physics.h"
#include "components/Transform.h"

namespace tZ
{

void SSpawn::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();

    for(const entt::entity& entity : registry.view<CTransform, CSpawner>())
    {
        CSpawner& spawner = GetComponent<CSpawner>(entity);
        const CTransform& transform = GetComponent<CTransform>(entity);

        spawner.SpawnTimer += deltaSeconds;
        if(spawner.SpawnTimer >= spawner.SpawnRate)
        {
            spawner.SpawnTimer = 0.f;
            entt::entity e = CreateEntity();
            CTransform entityTransform = transform;
            AddComponent(e, entityTransform);
            CFollow followComp{};
            AddComponent(e, followComp);
            CSphere sphereComponent{};
            sphereComponent.Color = FColor(0xFF0000FF);
            sphereComponent.Radius = .5f;
            AddComponent(e, sphereComponent);
            CPhysics2D physics{};
            physics.Speed = 10.f;
            physics.MaxSpeed = 10.f;
            AddComponent(e, physics);
            CEnemy enemy{};
            AddComponent(e, enemy);
            CNetwork network{};
            AddComponent(e, network);
        }
    }
}

}
