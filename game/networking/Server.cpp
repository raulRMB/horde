#include <enet/enet.h>
#include "raylib.h"
#include <iostream>
#include "Server.h"
#include "Game.h"
#include "NetworkDriver.h"
#include "NetMessage.h"
#include "systems/System.h"
#include "systems/moba/Navigation.h"
#include "components/Follow.h"
#include "components/Network.h"

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
     ENetEvent event;
     if(enet_host_service(server, &event, 0) > 0)
     {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            TraceLog(LOG_INFO, "CONNECTED!");
            InitialConnection* c = new InitialConnection;
            c->peer = event.peer;
            c->Type = ENetMsg::InitialConnection;
            NetworkDriver::GetInboundQueue().push((enet_uint8*)c);
        }
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            NetworkDriver::GetInboundQueue().push(event.packet->data);
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECTED!");
        }
     }

}

void Server::OnInboundMessage(ENetMsg msg, enet_uint8 *data) {
    switch (msg)
    {
        case ENetMsg::MoveTo: {
            NetMessageVector2 d = *(NetMessageVector2 *) data;
            if (System::Get<SNavigation>().IsValidPoint(d.pos)) {
                auto e = NetworkDriver::GetNetworkedEntities().Get(d.NetworkId);
                CFollow &followComponent = Game::GetRegistry().get<CFollow>(e);
                followComponent.FollowState = EFollowState::Dirty;
                followComponent.Index = 1;
                TraceLog(LOG_INFO, "x: %f y %f", d.pos.x, d.pos.y);
                followComponent.Goal = d.pos;
            }
            break;
        }
        case ENetMsg::InitialConnection: {
            InitialConnection x = *(InitialConnection *) data;
            OnConnect(x.peer);
            break;
        }
        default:
            TraceLog(LOG_INFO, "DEFAULT");
            break;
    }
}

entt::entity Server::CreateNetworkedEntity() {
    entt::entity e = Game::GetRegistry().create();
    CNetwork n = CNetwork{};
    Game::GetRegistry().emplace<CNetwork>(e, n);
    NetworkDriver::GetNetworkedEntities().Add(e);
    return e;
}

void Server::SendPlayerJoined(uint32_t netId) {
    NetPlayerJoined* res = new NetPlayerJoined{};
    res->Type = ENetMsg::PlayerJoined;
    res->NetworkId = netId;
    void* payload = (void*)res;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*res), ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections = NetworkDriver::GetConnections();
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Server::OnConnect(ENetPeer* peer) {
    Player* p = new Player;
    players.push_back(p);
    auto netId = NetworkDriver::GetNetworkedEntities().Get(p->GetEntity());
    SendPlayerJoined(netId);
    NetworkDriver::GetConnections().push_back(peer);
    ConnectResponse(peer, netId);
}

void Server::SendOutboundMessage(OutboundMessage msg) {
    for(ENetPeer* peer : msg.Connections) {
        enet_peer_send(peer, 0, msg.Packet);
    }
}

void Server::ConnectResponse(ENetPeer* peer, uint32_t netId) {
    NetConnectionResponse* res = new NetConnectionResponse{};
    res->Type = ENetMsg::ConnectionResponse;
    TraceLog(LOG_INFO, "owning entity networkID: %u", netId);
    res->NetworkId = netId;
    void* payload = (void*)res;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*res), ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections.push_back(peer);
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Server::Sync(entt::entity e, Transform& t, std::vector<ENetPeer*> c) {
    SyncTransform* st = new SyncTransform{};
    st->Type = ENetMsg::SyncTransform;
    st->NetworkId = NetworkDriver::GetNetworkedEntities().Get(e);
    st->t = t;
    void* payload = (void*)st;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*st), ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections = c;
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}