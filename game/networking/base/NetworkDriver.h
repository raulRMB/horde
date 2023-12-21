#ifndef NETWORKDRIVER_H
#define NETWORKDRIVER_H

#include "networking/Server.h"
#include "networking/Client.h"
#include "NetMessage.h"
#include "networking/base/NetworkedEntities.h"
#include "ThreadSafeQueue.h"
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

namespace tX
{

class NetworkDriver {

    NetworkDriver();
    bool useOwnThread = true;

    static NetworkDriver& Instance();

    NetworkDriver(const NetworkDriver&) = delete;
    NetworkDriver& operator=(const NetworkDriver&) = delete;

    class Client* client;
    class Server* server;
    NetworkedEntities networkedEntities;
    std::vector<ENetPeer*> connections;
    ThreadSafeQueue<IncomingMessage> inboundQueue;
    ThreadSafeQueue<OutboundMessage> outboundQueue;

public:
    void Init(long long periodMicroseconds);
    static void Start(long long periodMicroseconds) { Instance().Init(periodMicroseconds); };
    static class Server* GetServer();
    static class Client* GetClient();
    static ThreadSafeQueue<IncomingMessage>& GetInboundQueue() { return Instance().inboundQueue; };
    static ThreadSafeQueue<OutboundMessage>& GetOutboundQueue() { return Instance().outboundQueue; };
    static std::vector<ENetPeer*>& GetConnections() { return Instance().connections; };
    static void Process();
    static NetworkedEntities& GetNetworkedEntities();
};

}

#endif
