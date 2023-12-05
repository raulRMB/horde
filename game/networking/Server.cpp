#include <enet/enet.h>
#include "raylib.h"
#include "Server.h"
#include "Game.h"
#include "NetworkDriver.h"
#include "NetMessage.h"
#include "systems/System.h"
#include "systems/moba/Navigation.h"
#include "components/Follow.h"

#include "flatbuffers/flatbuffers.h"
#include "buffers/FlatBufferUtil.h"
#include "networking/buffers/Events_generated.h"

Server::Server() {
    if (enet_initialize() != 0) {
        TraceLog(LOG_INFO, "Failed to initialize ENet");
    }
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7777;
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        TraceLog(LOG_INFO, "Failed to create server");
        enet_deinitialize();
    }
    TraceLog(LOG_INFO, "Server Running");
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
            TraceLog(LOG_INFO, "DISCONNECTED!");
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
            Vector2 vec = Vector2{res->pos()->x(), res->pos()->y()};
            if (System::Get<SNavigation>().IsValidPoint(vec)) {
                auto e = NetworkDriver::GetNetworkedEntities().Get(res->netId());
                CFollow &followComponent = Game::GetRegistry().get<CFollow>(e);
                followComponent.FollowState = EFollowState::Dirty;
                followComponent.Index = 1;
                followComponent.Goal = vec;
                break;
            }
        }
    }
}

void Server::SendPlayerJoined(uint32_t netId) {
    flatbuffers::FlatBufferBuilder builder;
    Net::OnPlayerJoinedBuilder pjb(builder);
    pjb.add_netId(netId);
    auto pj = pjb.Finish();
    Send(builder, Net::Events::Events_OnPlayerJoined, pj.Union(), NetworkDriver::GetConnections());
}

void Server::OnConnect(ENetPeer* peer) {
    Player* p = new Player;
    players.push_back(p);
    auto netId = NetworkDriver::GetNetworkedEntities().Get(p->GetEntity());
    NetworkDriver::GetNetworkedEntities().SetOwner(netId, peer);
    SendPlayerJoined(netId);
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

    Transform& t = Game::GetRegistry().get<Transform>(NetworkDriver::GetNetworkedEntities().Get(netId));

//    Transform f = Transform {};
//    f.translation.x = -5;
    flatbuffers::Offset<Net::PlayerSpawn> ps = FlatBufferUtil::CreatePlayerSpawn(builder, t, netId);

    auto vec = builder.CreateVector(otherPlayers);
    Net::OnConnectionResponseBuilder crbuilder(builder);
    crbuilder.add_self(ps);
    crbuilder.add_others(vec);
    auto connectionResponse = crbuilder.Finish();
    Send(builder, Net::Events::Events_OnConnectionResponse, connectionResponse.Union(), FlatBufferUtil::CreatePeerVector(peer));
}

void Server::Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data, std::vector<ENetPeer*>& c) {
    auto header = CreateHeader(builder, type, data);
    builder.Finish(header);
    ENetPacket* packet = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections = c;
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Server::Sync(entt::entity e, Transform& t, std::vector<ENetPeer*> c) {
    flatbuffers::FlatBufferBuilder builder;
    auto x = FlatBufferUtil::CreateTransform(builder, t);
    Net::SyncTransformBuilder stb = Net::SyncTransformBuilder(builder);
    stb.add_netId(NetworkDriver::GetNetworkedEntities().Get(e));
    stb.add_transform(x);
    auto payload = stb.Finish();
    Send(builder, Net::Events::Events_SyncTransform, payload.Union(), c);
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}