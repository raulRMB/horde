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
#include "systems/moba/SNavigation.h"
#include "components/CFollow.h"
#include "flatbuffers/flatbuffers.h"
#include "buffers/FlatBufferUtil.h"
#include "components/CAttribute.h"
#include "networking/buffers/Events_generated.h"
#include "components/CCharacterAnimation.h"
#include "abilities/Projectile.h"
#include "util/Builder.h"

namespace tX
{

    Server::Server() {
    if(Game::IsServer()) {
        if (enet_initialize() != 0) {
            raylib::TraceLog(raylib::LOG_ERROR, "Failed to initialize ENet");
        }
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = NetworkDriver::GetPort();
        server = enet_host_create(&address, 32, 2, 0, 0);
        server->maximumPacketSize = 64 * 1024 * 1024;
        server->maximumWaitingData = 64 * 1024 * 1024;

        enet_peer_timeout(server->peers, ENET_PEER_TIMEOUT_LIMIT, ENET_PEER_TIMEOUT_MINIMUM, ENET_PEER_TIMEOUT_MAXIMUM);
        if (server == nullptr) {
            raylib::TraceLog(raylib::LOG_ERROR, "Failed to create server");
            enet_deinitialize();
        }
        raylib::TraceLog(raylib::LOG_INFO, "Server Running");
    }
}

void Server::Loop() {
    if(server == nullptr) {
        return;
    }
     ENetEvent event;
     if(enet_host_service(server, &event, 0) > 0)
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
            auto res = header->Event_as_OnConnection();
            auto netId = res->netId();
            if(netId > 0) {
                //OnReconnect(peer, netId);
            } else {
                OnConnect(peer);
            }
            break;
        }
        case Net::Events_OnMoveTo: {
            auto res = header->Event_as_OnMoveTo();
            v2 vec = v2{res->pos()->x(), res->pos()->y()};
            if (System::Get<SNavigation>().IsValidPoint(vec)) {
                if(NetworkDriver::GetNetworkedEntities().Exists(res->netId())) {
                    auto e = NetworkDriver::GetNetworkedEntities().Get(res->netId());
                    CFollow &followComponent = Game::GetRegistry().get<CFollow>(e);
                    followComponent.FollowState = EFollowState::Dirty;
                    followComponent.Index = 1;
                    followComponent.Goal = vec;
                }
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
    auto fbt = FlatBufferUtil::CreateTransform(builder, t);
    Net::OnPlayerJoinedBuilder pjb(builder);
    pjb.add_netId(netId);
    pjb.add_transform(fbt);
    auto pj = pjb.Finish();
    Send(builder, Net::Events::Events_OnPlayerJoined, pj.Union(), NetworkDriver::GetConnections(), ENET_PACKET_FLAG_RELIABLE);
}

void Server::SendSpawnEntity(uint32_t netId, uint32_t entityType, CTransform& t) {
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
    flatbuffers::Offset<Net::SpawnProjectile> msg = FlatBufferUtil::CreateSpawnProjectile(builder, netId, pos, dir, speed, lifetime);
    Send(builder, Net::Events::Events_SpawnProjectile, msg.Union(), NetworkDriver::GetConnections(), ENET_PACKET_FLAG_RELIABLE);
}

void Server::OnConnect(ENetPeer* peer) {
    CTransform t = CTransform{};
    t.Scale = v3{0.055, 0.055, 0.055};
    entt::entity e = Builder::Player(t);
    players.push_back(e);
    auto netId = NetworkDriver::GetNetworkedEntities().Get(e);
    NetworkDriver::GetNetworkedEntities().SetOwner(netId, peer);
    SendPlayerJoined(netId, Game::GetRegistry().get<CTransform>(e));
    NetworkDriver::GetConnections().push_back(peer);
    SendConnectResponse(peer, netId);
}

int packetsSent = 0;
void Server::SendOutboundMessage(OutboundMessage msg) {
    for(ENetPeer* peer : msg.Connections) {
        packetsSent++;
        auto channel = msg.Packet->flags == ENET_PACKET_FLAG_RELIABLE ? 1 : 0;
        enet_peer_send(peer, channel, msg.Packet);
    }
}

void Server::SendConnectResponse(ENetPeer* peer, uint32_t netId) {
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
    auto header = CreateHeader(builder, type, data);
    builder.Finish(header);
    ENetPacket* packet = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), flag);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections = c;
    NetworkDriver::GetOutboundQueue().push(msg);
    builder = flatbuffers::FlatBufferBuilder();
}

std::vector<flatbuffers::Offset<Net::SyncTransform>> transformsToSync;
std::vector<flatbuffers::Offset<Net::SyncAttributeComponent>> attributesToSync;
std::vector<flatbuffers::Offset<Net::SyncCharacterAnimState>> animsToSync;

void Server::Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c) {
    auto x = FlatBufferUtil::CreateTransform(builder, t);
    Net::SyncTransformBuilder stb = Net::SyncTransformBuilder(builder);
    u32 netId = NetworkDriver::GetNetworkedEntities().Get(e);
    stb.add_netId(netId);
    stb.add_transform(x);
    auto payload = stb.Finish();
    transformsToSync.push_back(payload);
}

void Server::SendSync(std::vector<ENetPeer*> c) {
        if(!transformsToSync.empty() || !attributesToSync.empty() || !animsToSync.empty()) {
            auto t = builder.CreateVector(transformsToSync);
            auto a = builder.CreateVector(attributesToSync);
            auto anim = builder.CreateVector(animsToSync);
            Net::SyncBuilder stb = Net::SyncBuilder(builder);
            stb.add_transforms(t);
            stb.add_attributes(a);
            stb.add_anims(anim);
            auto payload = stb.Finish();
            Send(builder, Net::Events::Events_Sync, payload.Union(), c, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
            transformsToSync.clear();
            attributesToSync.clear();
            animsToSync.clear();
        }
}

void Server::Sync(entt::entity e, CAttributeSet& ac, std::vector<ENetPeer*> c) {
    auto x = FlatBufferUtil::CreateSyncAttributes(builder, ac, NetworkDriver::GetNetworkedEntities().Get(e));
    attributesToSync.push_back(x);
}

void Server::Sync(entt::entity e, CCharacterAnimation& ca, std::vector<ENetPeer*> c)
{
    auto msg = Net::CreateSyncCharacterAnimState(builder, NetworkDriver::GetNetworkedEntities().Get(e), (int)ca.AnimState);
    animsToSync.push_back(msg);
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}

}