#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include <raylib.h>
#include "NetMessage.h"

class Server {
public:
    ENetHost* server;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(ENetMsg msg, enet_uint8 *data);
    void ConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(OutboundMessage msg);

    void Sync(entt::entity e, Transform t, std::vector<ENetPeer*> c);
};

#endif