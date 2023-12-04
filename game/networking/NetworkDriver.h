#ifndef NETWORKDRIVER_H
#define NETWORKDRIVER_H

#include "networking/Server.h"
#include "networking/Client.h"
#include "networking/base/NetworkedEntities.h"
#include <queue>

namespace tZ
{

class NetworkDriver {

    NetworkDriver();

    static NetworkDriver& Instance();

    NetworkDriver(const NetworkDriver&) = delete;
    NetworkDriver& operator=(const NetworkDriver&) = delete;

    bool isServer;
    bool offlineMode = false;

    Client* client;
    Server* server;
    NetworkedEntities networkedEntities;
    std::vector<ENetPeer*> connections;
    std::queue<enet_uint8*> inboundQueue;
    std::queue<NetOutboundMessage> outboundQueue;

public:
    void Init(long long periodMicroseconds);
    static void Start(long long periodMicroseconds) { Instance().Init(periodMicroseconds); };
    static bool IsServer();
    static bool IsOfflineMode();
    static Server* GetServer();
    static Client* GetClient();
    static std::queue<enet_uint8*>& GetInboundQueue() { return Instance().inboundQueue; };
    static std::queue<NetOutboundMessage>& GetOutboundQueue() { return Instance().outboundQueue; };
    static std::vector<ENetPeer*>& GetConnections() { return Instance().connections; };
    static void ProcessQueues();
    static NetworkedEntities& GetNetworkedEntities();
    static void SetIsServer(bool isServer);
};

}

#endif
