#ifndef NET_MESSAGES
#define NET_MESSAGES

#include <raylib.h>
#include <entt/entt.hpp>

enum class ENetMsg
{
    None = 0,
    InitialConnection,
    ConnectionResponse,
    MoveTo,
    SyncTransform
};

struct NetMessage
{
    ENetMsg Type = ENetMsg::None;
    NetMessage(ENetMsg type = ENetMsg::None) : Type(type) {}
};

struct OutboundMessage {
    ENetPacket* Packet;
    std::vector<ENetPeer*> Connections;
};

struct InitialConnection : public NetMessage {
    ENetPeer* peer;
};

struct SyncTransform : public NetMessage {
    u_int32_t NetworkId;
    Transform t;
};

struct NetConnectionResponse : public NetMessage
{
    u_int32_t NetworkId;
};

struct NetMessageTransform : public NetMessage
{
    u_int32_t NetworkId;
    Vector2 pos;
    // NetUpdateMessage(u32 entity = 0, Transform transform = Transform{}) : NetMessage(ENetMsg::UpdateEntity),
    //     EntityNetworkId(entity), Transform(transform) {}
};

#endif