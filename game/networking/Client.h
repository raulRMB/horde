#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include <enet/enet.h>
#include "entt/entt.hpp"
#include "raylib.h"
#include "NetMessage.h"

class Client {
public:
    ENetHost* client;
    ENetPeer* peer;
    Client();
    void Loop();
    void SendMoveTo(Vector2 pos, u_int32_t NetworkId);
    void OnInboundMessage(ENetMsg msg, enet_uint8 *data);
    void SendOutboundMessage(ENetPacket* packet);
    void flush() { enet_host_flush(client); }
    void Close();
};

#endif
