#include "FlatBufferUtil.h"

namespace raylib
{
#include "raylib.h"
}
#include "../Server.h"
#include "components/Follow.h"
#include "flatbuffers/flatbuffers.h"
#include "networking/buffers/Events_generated.h"
#include "components/Attribute.h"
#include "networking/NetworkDriver.h"
#include "components/Transform.h"

namespace tZ
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
    return x;
}

flatbuffers::Offset<Net::PlayerSpawn> FlatBufferUtil::CreatePlayerSpawn(flatbuffers::FlatBufferBuilder &builder, CTransform& t, uint32_t netId) {
    return Net::CreatePlayerSpawn(builder, netId, CreateTransform(builder, t));
}

flatbuffers::Offset<Net::Vector3> FlatBufferUtil::CreateVector3(flatbuffers::FlatBufferBuilder &builder, v3 v) {
    return Net::CreateVector3(builder, v.x, v.y, v.z);
}

flatbuffers::Offset<Net::Vector4> FlatBufferUtil::CreateVector4(flatbuffers::FlatBufferBuilder &builder, v4 v) {
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

flatbuffers::Offset<Net::Transform> FlatBufferUtil::CreateTransform(flatbuffers::FlatBufferBuilder &builder, CTransform& t) {
    t.Position.x = 0;
    auto positiom = CreateVector3(builder, t.Position);
    auto scale = CreateVector3(builder, t.Scale);
    auto rotation = CreateVector4(builder, t.VRotation);
    return Net::CreateTransform(builder, positiom, scale, rotation);
}

flatbuffers::Offset<Net::Attribute> FlatBufferUtil::CreateAttribute(flatbuffers::FlatBufferBuilder &builder, FAttribute& attr) {
    auto name = builder.CreateString(attr.id);
    return Net::CreateAttribute(builder, name, attr.get(), attr.max);
}

flatbuffers::Offset<Net::SyncAttributeComponent> FlatBufferUtil::CreateSyncAttributes(flatbuffers::FlatBufferBuilder &builder, CAttributes& ac, uint32_t netId) {
    auto vec = std::vector<flatbuffers::Offset<Net::Attribute>>();
    for(auto attr : ac.attributes) {
        vec.push_back(CreateAttribute(builder, attr));
    }
    auto v = builder.CreateVector(vec);
    return Net::CreateSyncAttributeComponent(builder, netId, v);
}

}