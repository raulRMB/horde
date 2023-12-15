#include "GarbageManager.h"

#include "networking/base/NetworkDriver.h"

namespace tZ
{

void SGarbageManager::Update(f32 deltaSeconds)
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity e : ToDestroy)
    {
        if(const CTransform* found = registry.try_get<CTransform>(e); found != nullptr)
            registry.destroy(e);

        if(NetworkDriver::GetNetworkedEntities().Exists(e))
            NetworkDriver::GetNetworkedEntities().Remove(e);
    }
}

}
