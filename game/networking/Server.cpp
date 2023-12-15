#include <enet/enet.h>
namespace raylib
{
#include "raylib.h"
}
#include "Server.h"
#include "Game.h"
#include "networking/base/NetworkDriver.h"
#include "networking/base/NetMessage.h"
#include "systems/System.h"
#include "systems/moba/Navigation.h"
#include "components/Follow.h"
#include "flatbuffers/flatbuffers.h"
#include "buffers/FlatBufferUtil.h"
#include "components/Attribute.h"
#include "networking/buffers/Events_generated.h"
#include "components/CharacterAnimation.h"
#include "abilities/Projectile.h"

namespace tZ
{

Server::Server() {
    if (enet_initialize() != 0) {
        raylib::TraceLog(raylib::LOG_INFO, "Failed to initialize ENet");
    }
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7777;
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        raylib::TraceLog(raylib::LOG_INFO, "Failed to create server");
        enet_deinitialize();
    }
    raylib::TraceLog(raylib::LOG_INFO, "Server Running");
}

void Server::Loop() {
    if(server == nullptr) {
        return;
    }
     ENetEvent event;
     if(enet_host_service(server, &event, 2000) > 0)
     {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {}
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            IncomingMessage msg;
            enet_uint8* dataCopy = new enet_uint8[event.packet->dataLength];
            std::memcpy(dataCopy, event.packet->data, event.packet->dataLength);
            msg.data = dataCopy;
            msg.peer = event.peer;
            NetworkDriver::GetInboundQueue().push(msg);
            enet_packet_destroy(event.packet);
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            raylib::TraceLog(raylib::LOG_INFO, "DISCONNECTED!");
        }
     }
}

void Server::OnInboundMessage(const Net::Header* header, ENetPeer* peer) {
    switch (header->Event_type()) {
        case Net::Events_OnConnection: {
            OnConnect(peer);
            break;
        }
        case Net::Events_OnMoveTo: {
            auto res = header->Event_as_OnMoveTo();
            v2 vec = v2{res->pos()->x(), res->pos()->y()};
            if (System::Get<SNavigation>().IsValidPoint(vec)) {
                auto e = NetworkDriver::GetNetworkedEntities().Get(res->netId());
                CFollow &followComponent = Game::GetRegistry().get<CFollow>(e);
                followComponent.FollowState = EFollowState::Dirty;
                followComponent.Index = 1;
                followComponent.Goal = vec;
                break;
            }
        }
        case Net::Events_TriggerAbility: {
            const Net::TriggerAbility* res = header->Event_as_TriggerAbility();
            entt::entity e = NetworkDriver::GetNetworkedEntities().Get(res->netId());
            CCharacterAnimation& charAnim =  Game::GetRegistry().get<CCharacterAnimation>(e);
            charAnim.bOverrideAnim = true;
            charAnim.EndAnimTime = 1.5f;
            charAnim.AnimState = ECharacterAnimState::Attacking1;
            charAnim.bNeedsNetSync = true;

            const v3 c = v3{res->targetVec()->x(), res->targetVec()->y(), res->targetVec()->z()};
            const CTransform clickPoint = CTransform{c};
            const CTransform& t = Game::GetRegistry().get<CTransform>(e);
            Projectile(e, clickPoint.Position, t.Position, 50, 1.2);

            const v3 dir = c - t.Position;
            const v2 dir2d = v2{dir.x, dir.z};

            SendSpawnProjectile(res->netId(), v2{t.Position.x, t.Position.z}, dir2d, 50, 1.2);

            break;
        }
    }
}

void Server::SendPlayerJoined(uint32_t netId, CTransform& t) {
    flatbuffers::FlatBufferBuilder builder;
    auto fbt = FlatBufferUtil::CreateTransform(builder, t);
    Net::OnPlayerJoinedBuilder pjb(builder);
    pjb.add_netId(netId);
    pjb.add_transform(fbt);
    auto pj = pjb.Finish();
    Send(builder, Net::Events::Events_OnPlayerJoined, pj.Union(), NetworkDriver::GetConnections(), ENET_PACKET_FLAG_RELIABLE);
}

void Server::SendSpawnEntity(uint32_t netId, uint32_t entityType, CTransform& t) {
    flatbuffers::FlatBufferBuilder builder;
    auto fbt = FlatBufferUtil::CreateTransform(builder, t);
    Net::SpawnEntityBuilder seb(builder);
    seb.add_netid(netId);
    seb.add_location(fbt);
    seb.add_entityType(entityType);
    auto pj = seb.Finish();
    Send(builder, Net::Events::Events_SpawnEntity, pj.Union(), NetworkDriver::GetConnections(), ENET_PACKET_FLAG_RELIABLE);
}

void Server::SendSpawnProjectile(u32 netId, v2 pos, v2 dir, float speed, float lifetime)
{
    flatbuffers::FlatBufferBuilder builder;
    flatbuffers::Offset<Net::SpawnProjectile> msg = FlatBufferUtil::CreateSpawnProjectile(builder, netId, pos, dir, speed, lifetime);
    Send(builder, Net::Events::Events_SpawnProjectile, msg.Union(), NetworkDriver::GetConnections(), ENET_PACKET_FLAG_RELIABLE);
}

void Server::OnConnect(ENetPeer* peer) {
    Player* p = new Player;
    players.push_back(p);
    auto netId = NetworkDriver::GetNetworkedEntities().Get(p->GetEntity());
    NetworkDriver::GetNetworkedEntities().SetOwner(netId, peer);
    SendPlayerJoined(netId, p->GetComponent<CTransform>());
    NetworkDriver::GetConnections().push_back(peer);
    SendConnectResponse(peer, netId);
}

int packetsSent = 0;
void Server::SendOutboundMessage(OutboundMessage msg) {
    for(ENetPeer* peer : msg.Connections) {
        packetsSent++;
        enet_peer_send(peer, 0, msg.Packet);
    }
}

void Server::SendConnectResponse(ENetPeer* peer, uint32_t netId) {
    flatbuffers::FlatBufferBuilder builder;
    auto otherPlayers = FlatBufferUtil::GetOtherPlayers(builder, peer);
    CTransform& t = Game::GetRegistry().get<CTransform>(NetworkDriver::GetNetworkedEntities().Get(netId));
    flatbuffers::Offset<Net::PlayerSpawn> ps = FlatBufferUtil::CreatePlayerSpawn(builder, t, netId);

    auto vec = builder.CreateVector(otherPlayers);
    Net::OnConnectionResponseBuilder crbuilder(builder);
    crbuilder.add_self(ps);
    crbuilder.add_others(vec);
    auto connectionResponse = crbuilder.Finish();
    Send(builder, Net::Events::Events_OnConnectionResponse, connectionResponse.Union(), FlatBufferUtil::CreatePeerVector(peer), ENET_PACKET_FLAG_RELIABLE);
}

void Server::Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data, std::vector<ENetPeer*>& c,
                  ENetPacketFlag flag) {
    auto header = CreateHeader(builder, Util::GenerateTimestamp(), type, data);
    builder.Finish(header);
    ENetPacket* packet = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), flag);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections = c;
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Server::Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c) {
    flatbuffers::FlatBufferBuilder builder;
    auto x = FlatBufferUtil::CreateTransform(builder, t);
    Net::SyncTransformBuilder stb = Net::SyncTransformBuilder(builder);
    stb.add_netId(NetworkDriver::GetNetworkedEntities().Get(e));
    stb.add_transform(x);
    auto payload = stb.Finish();
    Send(builder, Net::Events::Events_SyncTransform, payload.Union(), c, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
}

void Server::Sync(entt::entity e, CAttributeSet& ac, std::vector<ENetPeer*> c) {
    flatbuffers::FlatBufferBuilder builder;
    auto x = FlatBufferUtil::CreateSyncAttributes(builder, ac, NetworkDriver::GetNetworkedEntities().Get(e));
    Send(builder, Net::Events::Events_SyncAttributeComponent, x.Union(), c, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
}

void Server::Sync(entt::entity e, CCharacterAnimation& ca, std::vector<ENetPeer*> c)
{
    flatbuffers::FlatBufferBuilder builder;
    auto msg = Net::CreateSyncCharacterAnimState(builder, NetworkDriver::GetNetworkedEntities().Get(e), (int)ca.AnimState);
    Send(builder, Net::Events::Events_SyncCharacterAnimState, msg.Union(), c, ENET_PACKET_FLAG_RELIABLE);

}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}

}