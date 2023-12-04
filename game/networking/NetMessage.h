#ifndef NET_MESSAGES
#define NET_MESSAGES

#include <entt/entt.hpp>
#include "components/Transform.h"

namespace tZ
{

enum class ENetMsg
{
    None = 0,
    InitialConnection,
    ConnectionResponse,
    MoveTo,
    SyncTransform,
    PlayerJoined
};

struct NetMessage
{
    ENetMsg Type = ENetMsg::None;
    NetMessage(ENetMsg type = ENetMsg::None) : Type(type) {}
};

struct NetOutboundMessage {
    ENetPacket* Packet;
    std::vector<ENetPeer*> Connections;
};

struct NetInitialConnection : public NetMessage {
    ENetPeer* peer;
};

struct NetSyncTransform : public NetMessage {
    uint32_t NetworkId;
    CTransform t;
};

struct NetPlayerJoined : public NetMessage
{
    uint32_t NetworkId;
};

struct NetConnectionResponse : public NetMessage
{
    uint32_t NetworkId;
};

struct NetMessageVector2 : public NetMessage
{
    uint32_t NetworkId;
    v2 Vector;
};

}

#endif