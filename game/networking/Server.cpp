#include <enet/enet.h>
#include "raylib.h"
#include "Server.h"

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
            auto peer = event.peer;

        }
        else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            TraceLog(LOG_INFO, "MESSAGE!");
        }
        else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECTED!");
        }
     }
}

void Server::Close() {
    enet_host_destroy(server);
    enet_deinitialize();
}