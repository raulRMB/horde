#include "Networking.h"
#include "entt/entt.hpp"
#include "components/Network.h"
#include "Game.h"
#include "networking/NetworkDriver.h"

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
    }
}

}