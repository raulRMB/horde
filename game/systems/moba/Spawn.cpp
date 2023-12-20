#include "Spawn.h"
#include "components/CSpawner.h"
#include "components/CFollow.h"
#include "components/CEnemy.h"
#include "components/CShapes.h"
#include "components/CPhysics.h"
#include "components/CTransform.h"
#include "components/CAttribute.h"
#include "util/Builder.h"
#include "networking/base/NetworkDriver.h"

namespace tX
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
            entt::entity e = Builder::Enemy(transform);
            if(Game::IsServer()) {
                auto netId = NetworkDriver::GetNetworkedEntities().Add(e);
                NetworkDriver::GetServer()->SendSpawnEntity(netId, 1, transform);
            }
        }
    }
}

}
