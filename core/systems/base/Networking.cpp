#include "Networking.h"
#include "entt/entt.hpp"
#include "components/Network.h"
#include "Game.h"
#include <raymath.h>
#include "networking/NetworkDriver.h"

void SNetworking::Update(float deltaSeconds)
{
    if(NetworkDriver::IsServer()) {
        for (const entt::entity &entity: Game::GetRegistry().view<Transform, CNetwork>()) {
            Transform t = Game::GetRegistry().get<Transform>(entity);
            CNetwork& cn = Game::GetRegistry().get<CNetwork>(entity);

            if(abs(Vector3Distance(t.translation, cn.prevTransform.translation)) > 0.2) {
                cn.prevTransform.translation = t.translation;
                NetworkDriver::GetServer()->Sync(entity, t, NetworkDriver::GetConnections());
            }
        }
    }
}
