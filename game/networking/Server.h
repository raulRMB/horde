#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include "NetMessage.h"
#include "Player.h"
#include "components/Transform.h"

namespace tZ
{

class Server
{
public:
    ENetHost* server;
    std::vector<Player*> players;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(ENetMsg msg, enet_uint8 *data);
    void ConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(NetOutboundMessage msg);
    void SendPlayerJoined(uint32_t netId);
    entt::entity CreateNetworkedEntity();


    void flush() { enet_host_flush(server); }
    void Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c);
};

}
#endif