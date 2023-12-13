#ifndef NET_MESSAGES
#define NET_MESSAGES

#include "entt/entt.hpp"
#include <vector>

struct IncomingMessage {
    ENetPeer* peer;
    enet_uint8* data;
};

struct OutboundMessage {
    ENetPacket* Packet;
    std::vector<ENetPeer*> Connections;
};

#endif