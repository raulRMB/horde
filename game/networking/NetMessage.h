#ifndef NET_MESSAGES
#define NET_MESSAGES

#include <raylib.h>
#include <entt/entt.hpp>

enum class ENetMsg
{
    None = 0,
    ConnectionResponse,
    MoveTo,
};

struct NetMessage
{
    ENetMsg Type = ENetMsg::None;
    NetMessage(ENetMsg type = ENetMsg::None) : Type(type) {}
};

struct NetConnectionResponse : public NetMessage
{
    entt::entity NetworkId;
};

struct NetMessageTransform : public NetMessage
{
    int EntityNetworkId;
    Vector2 pos;
    // NetUpdateMessage(u32 entity = 0, Transform transform = Transform{}) : NetMessage(ENetMsg::UpdateEntity),
    //     EntityNetworkId(entity), Transform(transform) {}
};

#endif