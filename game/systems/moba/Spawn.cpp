#include "Spawn.h"
#include "components/Spawner.h"
#include "components/Follow.h"
#include "components/EnemyComponent.h"
#include "components/Network.h"
#include "components/Shapes.h"
#include "components/Physics.h"
#include "components/Transform.h"
#include "factories/Factory.h"
#include "networking/base/NetworkDriver.h"

namespace tZ
{

void SSpawn::Update(float deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();

    for(const entt::entity& entity : registry.view<CTransform, CSpawner>())
    {
        CSpawner& spawner = GetComponent<CSpawner>(entity);
        CTransform& transform = GetComponent<CTransform>(entity);

        spawner.SpawnTimer += deltaSeconds;
        if(spawner.SpawnTimer >= spawner.SpawnRate)
        {
            spawner.SpawnTimer = 0.f;
            const entt::entity e = Factory::Build(EBuilderType::Enemy, transform);
            const u32 netId = NetworkDriver::GetNetworkedEntities().Add(e);
            NetworkDriver::GetServer()->SendSpawnEntity(netId, (u32)EBuilderType::Enemy, transform);
        }
    }
}

}
