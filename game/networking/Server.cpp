#include <enet/enet.h>
#include "raylib.h"
#include <iostream>
#include "Server.h"
#include "Game.h"
#include "NetMessage.h"

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
            Game::OnConnect(event.peer);
        }
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            ENetMsg type = *(ENetMsg *) event.packet->data;
            switch (type)
            {
                case ENetMsg::MoveTo:
                    NetMessageTransform msg = *(NetMessageTransform *) event.packet->data;
                    TraceLog(LOG_INFO, "x: %f, y: %f", msg.pos.x, msg.pos.y);
//                    Game::GetRegistry().get<>()
                    break;
            }
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECTED!");
        }
     }
}

void Server::ConnectResponse(ENetPeer* peer, entt::entity connection) {
    NetConnectionResponse* res = new NetConnectionResponse{};
    res->Type = ENetMsg::ConnectionResponse;
    res->NetworkId = connection;
    void* payload = (void*)res;
    ENetPacket* packet = enet_packet_create(payload, sizeof(payload), ENET_PACKET_FLAG_RELIABLE);
    TraceLog(LOG_INFO, "entt: %d", res->NetworkId);
    enet_peer_send(peer, 0, packet);
    //Game::SpawnPlayer(res->NetworkId);
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}