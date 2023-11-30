#include <enet/enet.h>
#include <iostream>
#include <cstring>
#include "raylib.h"
#include "Client.h"
#include "Game.h"
#include "NetworkDriver.h"
#include "NetMessage.h"

Client::Client() {
    if (enet_initialize() != 0) {
        TraceLog(LOG_INFO, "Failed to initialize ENet");
    }
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (client == nullptr) {
        TraceLog(LOG_INFO, "Failed to create server");
        enet_deinitialize();
    }
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 7777;
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        TraceLog(LOG_INFO, "No available peers for initiating an ENet connection");
        enet_host_destroy(client);
        enet_deinitialize();
    }
    TraceLog(LOG_INFO, "Client Running");
}

void Client::SendMoveTo(Vector2 pos, u_int32_t NetworkId) {
    NetMessageVector2* mt = new NetMessageVector2{};
    mt->pos = pos;
    mt->Type = ENetMsg::MoveTo;
    mt->NetworkId = NetworkId;
    void* payload = (void*)mt;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*mt), ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections.push_back(peer);
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Client::Loop() {
    ENetEvent event;
    if(enet_host_service(client, &event, 0) > 0)
    {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            TraceLog(LOG_INFO, "CONNECT!");
        } else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            NetworkDriver::GetInboundQueue().push(event.packet->data);
        } else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECT!");
        }
    }
}

void Client::OnInboundMessage(ENetMsg msg, enet_uint8 *data) {
    switch (msg) {
        case ENetMsg::ConnectionResponse: {
            NetConnectionResponse msg = *(NetConnectionResponse *) data;
            Game::SpawnPlayer(msg.NetworkId);
            TraceLog(LOG_INFO, "connected owning entity networkID: %u", msg.NetworkId);
            break;
        }
        case ENetMsg::SyncTransform: {
            SyncTransform x = *(SyncTransform *) data;
            Transform& t = Game::GetRegistry().get<Transform>(NetworkDriver::GetNetworkedEntities().Get(x.NetworkId));
            t.translation = x.t.translation;
            t.rotation = x.t.rotation;
            t.scale = x.t.scale;
            break;
        }
    }
}

void Client::SendOutboundMessage(ENetPacket* packet) {
    enet_peer_send(peer, 0, packet);
}

void Client::Close() {
    enet_host_destroy(client);
    enet_deinitialize();
}