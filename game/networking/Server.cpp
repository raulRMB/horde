#include <enet/enet.h>
#include <iostream>
#include "Server.h"
#include "Game.h"
#include "NetworkDriver.h"
#include "NetMessage.h"
#include "systems/System.h"
#include "systems/moba/Navigation.h"
#include "components/Follow.h"
#include "components/Network.h"

namespace tZ
{

Server::Server() {
    if (enet_initialize() != 0) {
        printf("Failed to initialize ENet");
    }
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 7777;
    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == nullptr) {
        printf("Failed to create server");
        enet_deinitialize();
    }
    printf("Server Running");
}

void Server::Loop() {
    ENetEvent event;
    if(enet_host_service(server, &event, 2000) > 0)
    {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("CONNECTED!");
            NetInitialConnection* c = new NetInitialConnection;
            c->peer = event.peer;
            c->Type = ENetMsg::InitialConnection;
            NetworkDriver::GetInboundQueue().push((enet_uint8*)c);
            //delete c;
        }
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            enet_uint8* dataCopy = new enet_uint8[event.packet->dataLength];
            // std::memcpy(dataCopy, event.packet->data, event.packet->dataLength);
            NetworkDriver::GetInboundQueue().push(dataCopy);
            enet_packet_destroy(event.packet);
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            printf("DISCONNECTED!");
        }
    }

}

void Server::OnInboundMessage(ENetMsg msg, enet_uint8 *data) {
    switch (msg)
    {
    case ENetMsg::MoveTo: {
            NetMessageVector2 d = *(NetMessageVector2 *) data;
            if (System::Get<SNavigation>().IsValidPoint(d.Vector)) {
                auto e = NetworkDriver::GetNetworkedEntities().Get(d.NetworkId);
                CFollow &followComponent = Game::GetRegistry().get<CFollow>(e);
                followComponent.FollowState = EFollowState::Dirty;
                followComponent.Index = 1;
                printf("x: %f y %f", d.Vector.x, d.Vector.y);
                followComponent.Goal = d.Vector;
            }
            break;
    }
    case ENetMsg::InitialConnection: {
            NetInitialConnection x = *(NetInitialConnection *) data;
            OnConnect(x.peer);
            break;
    }
    default:
        printf("DEFAULT");
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
    NetOutboundMessage msg = NetOutboundMessage{};
    msg.Packet = packet;
    msg.Connections = NetworkDriver::GetConnections();
    NetworkDriver::GetOutboundQueue().push(msg);
    //delete res;
}

void Server::OnConnect(ENetPeer* peer) {
    Player* p = new Player;
    players.push_back(p);
    auto netId = NetworkDriver::GetNetworkedEntities().Get(p->GetEntity());
    SendPlayerJoined(netId);
    NetworkDriver::GetConnections().push_back(peer);
    ConnectResponse(peer, netId);
}

int packetsSent = 0;
void Server::SendOutboundMessage(NetOutboundMessage msg) {
    for(ENetPeer* peer : msg.Connections) {
        packetsSent++;
        enet_peer_send(peer, 0, msg.Packet);
        printf("sent %d packets", packetsSent);
    }
}

void Server::ConnectResponse(ENetPeer* peer, uint32_t netId) {
    NetConnectionResponse* res = new NetConnectionResponse{};
    res->Type = ENetMsg::ConnectionResponse;
    printf("owning entity networkID: %u", netId);
    res->NetworkId = netId;
    void* payload = (void*)res;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*res), ENET_PACKET_FLAG_RELIABLE);
    NetOutboundMessage msg = NetOutboundMessage{};
    msg.Packet = packet;
    msg.Connections.push_back(peer);
    NetworkDriver::GetOutboundQueue().push(msg);
    //delete res;
}

void Server::Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c) {
    NetSyncTransform* st = new NetSyncTransform{};
    st->Type = ENetMsg::SyncTransform;
    st->NetworkId = NetworkDriver::GetNetworkedEntities().Get(e);
    st->t = t;
    void* payload = (void*)st;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*st), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    NetOutboundMessage msg = NetOutboundMessage{};
    msg.Packet = packet;
    msg.Connections = c;
    NetworkDriver::GetOutboundQueue().push(msg);
    //delete st;
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}

}