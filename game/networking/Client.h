#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include <enet/enet.h>
#include "entt/entt.hpp"
#include "raylib.h"

class Client {
public:
    ENetHost* client;
    ENetPeer* peer;
    Client();
    void Loop();
    void SendMoveTo(Vector2 pos, u_int32_t NetworkId);
    void Close();
};

#endif
