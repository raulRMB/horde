#include "Networking.h"
#include "entt/entt.hpp"
#include "components/Network.h"
#include "Game.h"
#include "networking/NetworkDriver.h"

void SNetworking::Update(float deltaSeconds)
{
    if(NetworkDriver::IsServer()) {
        for (const entt::entity &entity: Game::GetRegistry().view<Transform, CNetwork>()) {
            Transform t = Game::GetRegistry().get<Transform>(entity);
            NetworkDriver::GetServer()->Sync(entity, t, NetworkDriver::GetConnections());
        }
    }
}
