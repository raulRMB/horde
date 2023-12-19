#include "Spawn.h"
#include "components/CSpawner.h"
#include "components/CFollow.h"
#include "components/CEnemy.h"
#include "components/CShapes.h"
#include "components/CPhysics.h"
#include "components/CTransform.h"
#include "components/CAttribute.h"

namespace tX
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
            std::list<FAttribute> attributes;
            FAttribute Health = {
                    .id="health",
                    .base=500,
                    .max=1000,
                    .min=0,
            };
            attributes.push_back(Health);
            CAttributeSet ac{attributes};
            AddComponent(e, ac);
        }
    }
}

}
