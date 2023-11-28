#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include "entt/entt.hpp"

class Server {
public:
    ENetHost* server;
    Server();
    void Loop();
    void Close();

    void ConnectResponse(ENetPeer* peer, entt::entity connection);
};

#endif