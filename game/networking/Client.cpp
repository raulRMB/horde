#include <enet/enet.h>
#include <iostream>
#include <cstring>
#include "raylib.h"
#include "Client.h"
#include "Game.h"
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

void Client::SendMoveTo(Vector2 pos) {
    NetMessageTransform* mt = new NetMessageTransform{};
    mt->pos = pos;
    mt->Type = ENetMsg::MoveTo;
    mt->EntityNetworkId = 6;
    void* payload = (void*)mt;
    ENetPacket* packet = enet_packet_create(payload, sizeof(*mt), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Client::Loop() {
    ENetEvent event;
    if(enet_host_service(client, &event, 0) > 0)
    {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            TraceLog(LOG_INFO, "CONNECT!");
        } else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            Game::GetNetworkingQueue().push(event.packet->data);
        } else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECT!");
        }
    }
}

void Client::Close() {
    enet_host_destroy(client);
    enet_deinitialize();
}