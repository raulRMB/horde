#include "FlatBufferUtil.h"

#include "raylib.h"
#include "../Server.h"
#include "components/Follow.h"
#include "flatbuffers/flatbuffers.h"
#include "networking/buffers/Events_generated.h"
#include "networking/NetworkDriver.h"

std::vector<ENetPeer*>& FlatBufferUtil::CreatePeerVector(ENetPeer* peer) {
    std::vector<ENetPeer*>* x = new std::vector<ENetPeer*>();
    x->push_back(peer);
    return *x;
}

Transform FlatBufferUtil::NetTransformToTransform(const Net::Transform* t) {
    Transform x = Transform {};
    x.translation.x = t->translation()->x();
    x.translation.y = t->translation()->y();
    x.translation.z = t->translation()->z();
    return x;
}

flatbuffers::Offset<Net::PlayerSpawn> FlatBufferUtil::CreatePlayerSpawn(flatbuffers::FlatBufferBuilder &builder, Transform& t, uint32_t netId) {
    return Net::CreatePlayerSpawn(builder, netId, CreateTransform(builder, t));
}

flatbuffers::Offset<Net::Vector3> FlatBufferUtil::CreateVector3(flatbuffers::FlatBufferBuilder &builder, Vector3 v) {
    return Net::CreateVector3(builder, v.x, v.y, v.z);
}

flatbuffers::Offset<Net::Vector4> FlatBufferUtil::CreateVector4(flatbuffers::FlatBufferBuilder &builder, Vector4 v) {
    return Net::CreateVector4(builder, v.x, v.y, v.z, v.w);
}

std::vector<flatbuffers::Offset<Net::PlayerSpawn>> FlatBufferUtil::GetOtherPlayers(flatbuffers::FlatBufferBuilder &builder, ENetPeer* peer) {
    std::vector<flatbuffers::Offset<Net::PlayerSpawn>> otherPlayers;
    for(ENetPeer* connection : NetworkDriver::GetConnections()) {
        if(connection == peer) {
            continue;
        }
        auto netId = NetworkDriver::GetNetworkedEntities().GetOwner(connection);
        Transform& t = Game::GetRegistry().get<Transform>(NetworkDriver::GetNetworkedEntities().Get(netId));
        auto ps = FlatBufferUtil::CreatePlayerSpawn(builder, t, netId);
        otherPlayers.push_back(ps);
    }
    return otherPlayers;
}

flatbuffers::Offset<Net::Transform> FlatBufferUtil::CreateTransform(flatbuffers::FlatBufferBuilder &builder, Transform& t) {
    auto translation = CreateVector3(builder, t.translation);
    auto scale = CreateVector3(builder, t.scale);
    auto quat = CreateVector4(builder, t.rotation);
    return Net::CreateTransform(builder, translation, scale, quat);
}
