#ifndef HORDE_FLATBUFFERUTIL_H
#define HORDE_FLATBUFFERUTIL_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include <raylib.h>
#include "Player.h"
#include "Events_generated.h"

class FlatBufferUtil {
public:
    static flatbuffers::Offset<Net::Vector3> CreateVector3(flatbuffers::FlatBufferBuilder &builder, Vector3 v);
    static flatbuffers::Offset<Net::Vector4> CreateVector4(flatbuffers::FlatBufferBuilder &builder, Vector4 v);
    static flatbuffers::Offset<Net::Transform> CreateTransform(flatbuffers::FlatBufferBuilder &builder, Transform& t);
    static std::vector<ENetPeer*>& CreatePeerVector(ENetPeer* peer);
    static flatbuffers::Offset<Net::PlayerSpawn> CreatePlayerSpawn(flatbuffers::FlatBufferBuilder &builder, Transform& t, uint32_t netId);
    static std::vector<flatbuffers::Offset<Net::PlayerSpawn>> GetOtherPlayers(flatbuffers::FlatBufferBuilder &builder, ENetPeer* peer);
    static Transform NetTransformToTransform(const Net::Transform* t);
};

#endif //HORDE_FLATBUFFERUTIL_H
