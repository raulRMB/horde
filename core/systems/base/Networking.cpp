#include "Networking.h"
#include "entt/entt.hpp"
#include "components/Network.h"
#include "Game.h"
#include "networking/NetworkDriver.h"
#include "components/Attribute.h"

namespace tZ
{

void SNetworking::Update(float deltaSeconds)
{
    if(NetworkDriver::IsServer()) {
        for (const entt::entity &entity: Game::GetRegistry().view<CTransform, CNetwork>()) {
            CTransform t = Game::GetRegistry().get<CTransform>(entity);
            CNetwork& cn = Game::GetRegistry().get<CNetwork>(entity);

            if(abs(glm::distance(t.Position, cn.PrevTransform.Position)) > 0.2) {
                cn.PrevTransform.Position = t.Position;
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

}