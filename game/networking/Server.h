#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>

class Server {
public:
    ENetHost* server;
    Server();
    void Loop();
    void Close();
};

#endif