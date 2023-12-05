#ifndef SERVER_H
#define SERVER_H

#include <entt/entt.hpp>
#include <vector>
#include "NetMessage.h"
#include "components/Transform.h"

typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;
typedef uint8_t enet_uint8;

namespace tZ
{

class Server
{
public:
    ENetHost* server;
    std::vector<class Player*> players;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(ENetMsg msg, enet_uint8 *data);
    void ConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(NetOutboundMessage msg);
    void SendPlayerJoined(uint32_t netId);
    entt::entity CreateNetworkedEntity();


    void flush();
    void Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c);
};

}
#endif