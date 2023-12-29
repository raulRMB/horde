#include "FlatBufferUtil.h"

#include "../Server.h"
#include "components/CFollow.h"
#include "flatbuffers/flatbuffers.h"
#include "networking/buffers/Events_generated.h"
#include "components/CAttribute.h"
#include "networking/base/NetworkDriver.h"
#include "components/CTransform.h"
#include "Game.h"

namespace tX
{

std::vector<ENetPeer*>& FlatBufferUtil::CreatePeerVector(ENetPeer* peer) {
    std::vector<ENetPeer*>* x = new std::vector<ENetPeer*>();
    x->push_back(peer);
    return *x;
}

CTransform FlatBufferUtil::NetTransformToTransform(const Net::Transform* t) {
    CTransform x = CTransform {};
    x.Position.x = t->translation()->x();
    x.Position.y = t->translation()->y();
    x.Position.z = t->translation()->z();
    x.Scale.x = t->scale()->x();
    x.Scale.y = t->scale()->y();
    x.Scale.z = t->scale()->z();
    x.Rotation.x = t->quaternion()->x();
    x.Rotation.y = t->quaternion()->y();
    x.Rotation.z = t->quaternion()->z();
    x.Rotation.w = t->quaternion()->w();
    return x;
}

flatbuffers::Offset<Net::PlayerSpawn> FlatBufferUtil::CreatePlayerSpawn(flatbuffers::FlatBufferBuilder &builder, CTransform& t, uint32_t netId) {
    return Net::CreatePlayerSpawn(builder, netId, CreateTransform(builder, t));
}

flatbuffers::Offset<Net::Vector3> FlatBufferUtil::CreateVector3(flatbuffers::FlatBufferBuilder &builder, v3 v) {
    return Net::CreateVector3(builder, v.x, v.y, v.z);
}

flatbuffers::Offset<Net::Vector4> FlatBufferUtil::CreateVector4(flatbuffers::FlatBufferBuilder &builder, quat v) {
    return Net::CreateVector4(builder, v.x, v.y, v.z, v.w);
}

std::vector<flatbuffers::Offset<Net::PlayerSpawn>> FlatBufferUtil::GetOtherPlayers(flatbuffers::FlatBufferBuilder &builder, ENetPeer* peer) {
    std::vector<flatbuffers::Offset<Net::PlayerSpawn>> otherPlayers;
    for(ENetPeer* connection : NetworkDriver::GetConnections()) {
        if(connection == peer) {
            continue;
        }
        u32 netId = NetworkDriver::GetNetworkedEntities().GetOwner(connection);
        CTransform& t = Game::GetRegistry().get<CTransform>(NetworkDriver::GetNetworkedEntities().Get(netId));
        auto ps = FlatBufferUtil::CreatePlayerSpawn(builder, t, netId);
        otherPlayers.push_back(ps);
    }
    return otherPlayers;
}

flatbuffers::Offset<Net::Transform> FlatBufferUtil::CreateTransform(flatbuffers::FlatBufferBuilder &builder, CTransform& t)
{
    auto positiom = CreateVector3(builder, t.Position);
    auto scale = CreateVector3(builder, t.Scale);
    auto rotation = CreateVector4(builder, t.Rotation);
    return Net::CreateTransform(builder, positiom, scale, rotation);
}

flatbuffers::Offset<Net::Attribute> FlatBufferUtil::CreateAttribute(flatbuffers::FlatBufferBuilder &builder, FAttribute& attr) {
    flatbuffers::Offset<flatbuffers::String> name = builder.CreateString(attr.id);
    return Net::CreateAttribute(builder, name, attr.get(), attr.max);
}

flatbuffers::Offset<Net::SyncAttributeComponent> FlatBufferUtil::CreateSyncAttributes(flatbuffers::FlatBufferBuilder &builder, CAttributeSet& ac, uint32_t netId) {
    std::vector<flatbuffers::Offset<Net::Attribute>> vec = std::vector<flatbuffers::Offset<Net::Attribute>>();
    for(auto attr : ac.Attributes) {
        vec.push_back(CreateAttribute(builder, attr));
    }
    const flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Net::Attribute>>> v = builder.CreateVector(vec);
    return Net::CreateSyncAttributeComponent(builder, netId, v);
}

flatbuffers::Offset<Net::SpawnProjectile> FlatBufferUtil::CreateSpawnProjectile(flatbuffers::FlatBufferBuilder &builder,
    const u32 netId, const v2& pos, const v2& dir, const float speed, const float lifetime)
{
    const flatbuffers::Offset<Net::Vector2> posOffset = Net::CreateVector2(builder, pos.x, pos.y);
    const flatbuffers::Offset<Net::Vector2> dirOffset = Net::CreateVector2(builder, dir.x, dir.y);
    return Net::CreateSpawnProjectile(builder, netId, 0,posOffset, dirOffset, speed, lifetime);
}

}
