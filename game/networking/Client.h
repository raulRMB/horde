#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include "entt/entt.hpp"
#include "NetMessage.h"
#include "defines.h"

typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;
typedef struct _ENetPacket ENetPacket;
typedef uint8_t enet_uint8;

namespace tZ
{

class Client
{
public:
    ENetHost* client;
    ENetPeer* peer;
    Client();
    void Loop();
    void SendMoveTo(v2 pos, uint32_t NetworkId);
    void OnInboundMessage(ENetMsg msg, enet_uint8 *data);
    void SendOutboundMessage(ENetPacket* packet);
    void flush();
    void Close();
};

}

#endif
