#ifndef NETWORKDRIVER_H
#define NETWORKDRIVER_H

#include "networking/Server.h"
#include "networking/Client.h"
#include "networking/NetMessage.h"
#include "networking/base/NetworkedEntities.h"
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

class NetworkDriver {

    NetworkDriver();

    static NetworkDriver& Instance();

    NetworkDriver(const NetworkDriver&) = delete;
    NetworkDriver& operator=(const NetworkDriver&) = delete;

    bool isServer;
    bool offlineMode = true;

    Client* client;
    Server* server;
    NetworkedEntities networkedEntities;
    std::map<entt::entity, ENetPeer> connections;
    std::queue<enet_uint8*> networkingQueue;

public:
    void Init();
    static void Start() { Instance().Init(); };
    static bool IsServer();
    static bool IsOfflineMode();
    static Server* GetServer();
    static Client* GetClient();
    static void OnConnect(ENetPeer* peer);
    static std::queue<enet_uint8*>& GetNetworkingQueue() { return Instance().networkingQueue; };
    static void ProcessQueue();
    static NetworkedEntities& GetNetworkedEntities();
    static void SetIsServer(bool isServer);

};

#endif
