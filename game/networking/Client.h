#ifndef HORDE_CLIENT_H
#define HORDE_CLIENT_H

#include <enet/enet.h>

class Client {
public:
    ENetHost* client;
    ENetPeer* peer;
    Client();
    void Loop();
    void SendMoveTo(Vector2 pos);
    void Close();
};

#endif
