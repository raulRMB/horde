#ifndef HORDE_FLATBUFFERUTIL_H
#define HORDE_FLATBUFFERUTIL_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include "smartentity/Player.h"
#include "Events_generated.h"

namespace tZ
{

struct CTransform;
struct FAttribute;
struct CAttributes;

class FlatBufferUtil {
public:
    static flatbuffers::Offset<Net::Vector3> CreateVector3(flatbuffers::FlatBufferBuilder &builder, v3 v);
    static flatbuffers::Offset<Net::Vector4> CreateVector4(flatbuffers::FlatBufferBuilder &builder, quat v);
    static flatbuffers::Offset<Net::Transform> CreateTransform(flatbuffers::FlatBufferBuilder &builder, CTransform& t);
    static std::vector<ENetPeer*>& CreatePeerVector(ENetPeer* peer);
    static flatbuffers::Offset<Net::PlayerSpawn> CreatePlayerSpawn(flatbuffers::FlatBufferBuilder &builder, CTransform& t, uint32_t netId);
    static std::vector<flatbuffers::Offset<Net::PlayerSpawn>> GetOtherPlayers(flatbuffers::FlatBufferBuilder &builder, ENetPeer* peer);
    static CTransform NetTransformToTransform(const Net::Transform* t);
    static flatbuffers::Offset<Net::Attribute> CreateAttribute(flatbuffers::FlatBufferBuilder &builder, FAttribute& attr);
    static flatbuffers::Offset<Net::SyncAttributeComponent> CreateSyncAttributes(flatbuffers::FlatBufferBuilder &builder, CAttributes& ac, uint32_t netId);
    static flatbuffers::Offset<Net::SpawnProjectile> CreateSpawnProjectile(flatbuffers::FlatBufferBuilder &builder, u32 netId, const v2& pos, const v2& dir,
                                                                           const float speed, const float lifetime);
};

}

#endif //HORDE_FLATBUFFERUTIL_H
