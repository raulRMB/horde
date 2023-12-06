#include "Networking.h"
#include "entt/entt.hpp"
#include "components/Network.h"
#include "Game.h"
#include <raymath.h>
#include "networking/NetworkDriver.h"
#include "components/Attribute.h"

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
        for (const entt::entity &entity: Game::GetRegistry().view<CAttributes, CNetwork>()) {
            CAttributes& ac = Game::GetRegistry().get<CAttributes>(entity);
            if(ac.needsSync) {
                NetworkDriver::GetServer()->Sync(entity, ac, NetworkDriver::GetConnections());
                ac.needsSync = false;
            }
        }
    }
}
